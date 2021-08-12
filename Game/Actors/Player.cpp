#include "Player.h"
#include "Projectile.h"
#include "Math/MathUtils.h"
#include "Engine.h"
#include "Enemy.h"
#include "Astroid.h"
#include <memory>

Player::Player(const PhoenixEngine::Transform& transform, std::shared_ptr<PhoenixEngine::Shape> shape, float speed) : Actor(transform, shape), speed{speed}
{
	
}

void Player::Initialize()
{
	std::unique_ptr locator = std::make_unique<Actor>();
	locator->transform.localPosition = PhoenixEngine::Vector2{ -8, 0 };
	locator->transform.localRotation = PhoenixEngine::DegToRad(180);
	AddChild(std::move(locator));

	locator = std::make_unique<Actor>();
	locator->transform.localPosition = PhoenixEngine::Vector2{ 0, 5 };
	AddChild(std::move(locator));

	locator = std::make_unique<Actor>();
	locator->transform.localPosition = PhoenixEngine::Vector2{ 0, -5 };
	AddChild(std::move(locator));

	locator = std::make_unique<Actor>();
	locator->transform.localPosition = PhoenixEngine::Vector2{ 8, 0 };
	AddChild(std::move(locator));

	std::unique_ptr blaster = std::make_unique<Actor>(PhoenixEngine::Transform{}, scene->engine->Get<PhoenixEngine::ResourceSystem>()->Get<PhoenixEngine::Shape>("blasterupgrade.txt"));
	blaster->transform.localPosition = PhoenixEngine::Vector2{ 0, 0 };
	AddChild(std::move(blaster));

	std::unique_ptr engine = std::make_unique<Actor>(PhoenixEngine::Transform{}, scene->engine->Get<PhoenixEngine::ResourceSystem>()->Get<PhoenixEngine::Shape>("engineupgrade.txt"));
	engine->transform.localPosition = PhoenixEngine::Vector2{ -5, 0 };
	AddChild(std::move(engine));
}

void Player::Update(float dt)
{
	Actor::Update(dt);

	// Movement
	float thrust = 0;
	if (Core::Input::IsPressed('A')) transform.rotation -= 5 * dt;
	if (Core::Input::IsPressed('D')) transform.rotation += 5 * dt;
	if (Core::Input::IsPressed('W')) thrust = speed;

	// Acceleration
	PhoenixEngine::Vector2 acceleration;
	acceleration += PhoenixEngine::Vector2::Rotate(PhoenixEngine::Vector2::right, transform.rotation) * thrust;
	velocity += acceleration * dt;
	transform.position += velocity * dt;
	
	velocity *= 0.99f; // Drag
	
	transform.position.x = PhoenixEngine::Wrap(transform.position.x, 0.0f, 800.0f);
	transform.position.y = PhoenixEngine::Wrap(transform.position.y, 0.0f, 600.0f);

	// Fire
	fireTimer -= dt;
	if (fireTimer <= 0 && Core::Input::IsPressed(VK_SPACE))
	{
		fireTimer = fireRate;
		
		{
			PhoenixEngine::Transform t = children[1]->transform;
			t.scale = 2.0f;

			std::unique_ptr<Projectile> projectile = std::make_unique<Projectile>(t, scene->engine->Get<PhoenixEngine::ResourceSystem>()->Get<PhoenixEngine::Shape>("bulletshape.txt"), 600.0f);
			projectile->tag = "Player";
			scene->AddActor(std::move(projectile));
		}

		{
			PhoenixEngine::Transform t = children[2]->transform;
			t.scale = 2.0f;

			std::unique_ptr<Projectile> projectile = std::make_unique<Projectile>(t, scene->engine->Get<PhoenixEngine::ResourceSystem>()->Get<PhoenixEngine::Shape>("bulletshape.txt"), 600.0f);
			projectile->tag = "Player";
			scene->AddActor(std::move(projectile));
		}

		scene->engine->Get<PhoenixEngine::AudioSystem>()->PlayAudio("playerFire");
	}
	
		std::vector<PhoenixEngine::Color> colors = {PhoenixEngine::Color::yellow, PhoenixEngine::Color::white, PhoenixEngine::Color::orange};
		scene->engine->Get<PhoenixEngine::ParticleSystem>()->Create(children[0]->transform.position, 3, 2, colors, 50, children[0]->transform.rotation, PhoenixEngine::DegToRad(30));
}

void Player::OnCollision(Actor* actor)
{
	if (dynamic_cast<Projectile*>(actor) && actor->tag == "Enemy")
	{
		actor->destroy = true;
		--health;
		if (health < 1)
		{
			destroy = true;
			scene->engine->Get<PhoenixEngine::ParticleSystem>()->Create(transform.position, 200, 2, PhoenixEngine::Color::white, 50);
			scene->engine->Get<PhoenixEngine::AudioSystem>()->PlayAudio("explosion");
			PhoenixEngine::Event event;
			event.name = "PlayerDead";
			event.data = std::string("YOU ARE DEAD");
			scene->engine->Get<PhoenixEngine::EventSystem>()->Notify(event);
		}
		else
		{
			scene->engine->Get<PhoenixEngine::AudioSystem>()->PlayAudio("hurt");
		}
	}

	if (dynamic_cast<Enemy*>(actor) || dynamic_cast<Astroid*>(actor))
	{
		health = 0;
		destroy = true;
		scene->engine->Get<PhoenixEngine::ParticleSystem>()->Create(transform.position, 200, 2, PhoenixEngine::Color::white, 50);
		scene->engine->Get<PhoenixEngine::AudioSystem>()->PlayAudio("explosion");
		PhoenixEngine::Event event;
		event.name = "PlayerDead";
		event.data = std::string("YOU ARE DEAD");
		scene->engine->Get<PhoenixEngine::EventSystem>()->Notify(event);
	}
}

size_t Player::GetHealth()
{
	return this->health;
}

void Player::SetHealth(size_t health)
{
	this->health = health;
}

