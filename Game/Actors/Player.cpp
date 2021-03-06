#include "Player.h"
#include "Math/MathUtils.h"
#include "Engine.h"
#include "Projectile.h"
#include "Enemy.h"
#include "Asteroid.h"
#include <memory>

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
}

void Player::Update(float dt)
{
	Actor::Update(dt);

	// Movement
	float thrust = 0;
	if (scene->engine->Get<PhoenixEngine::InputSystem>()->GetKeyState(SDL_SCANCODE_A) == PhoenixEngine::InputSystem::eKeyState::Held) transform.rotation -= 5 * dt;
	if (scene->engine->Get<PhoenixEngine::InputSystem>()->GetKeyState(SDL_SCANCODE_D) == PhoenixEngine::InputSystem::eKeyState::Held) transform.rotation += 5 * dt;
	if (scene->engine->Get<PhoenixEngine::InputSystem>()->GetKeyState(SDL_SCANCODE_W) == PhoenixEngine::InputSystem::eKeyState::Held) thrust = speed;

	// Acceleration
	PhoenixEngine::Vector2 acceleration;
	acceleration += PhoenixEngine::Vector2::Rotate(PhoenixEngine::Vector2::right, transform.rotation) * thrust;
	velocity += acceleration * dt;
	transform.position += velocity * dt;
	
	velocity *= 0.99f; // Drag
	
	transform.position.x = PhoenixEngine::Wrap(transform.position.x, 0.0f, 800.0f);
	transform.position.y = PhoenixEngine::Wrap(transform.position.y, 0.0f, 600.0f);

	PhoenixEngine::SetFilePath("../../Build/Resources");
	// Fire
	
	fireTimer -= dt;
	if (fireTimer <= 0 && scene->engine->Get<PhoenixEngine::InputSystem>()->GetKeyState(SDL_SCANCODE_SPACE) == PhoenixEngine::InputSystem::eKeyState::Held)
	{
		fireTimer = fireRate;
		
		{
			PhoenixEngine::Transform t = children[1]->transform;
			t.scale = 2.0f;

			std::unique_ptr<Projectile> projectile = std::make_unique<Projectile>(t, scene->engine->Get<PhoenixEngine::ResourceSystem>()->Get<PhoenixEngine::Texture>("sprites/playerlaser.png", scene->engine->Get<PhoenixEngine::Renderer>()), 600.0f);
			projectile->tag = "Player";
			scene->AddActor(std::move(projectile));
		}

		{
			PhoenixEngine::Transform t = children[2]->transform;
			t.scale = 2.0f;

			std::unique_ptr<Projectile> projectile = std::make_unique<Projectile>(t, scene->engine->Get<PhoenixEngine::ResourceSystem>()->Get<PhoenixEngine::Texture>("sprites/playerlaser.png", scene->engine->Get<PhoenixEngine::Renderer>()), 600.0f);
			projectile->tag = "Player";
			scene->AddActor(std::move(projectile));
		}

		scene->engine->Get<PhoenixEngine::AudioSystem>()->PlayAudio("playerFire");
	}
	
	// Create exhust on the ship.
	std::shared_ptr<PhoenixEngine::Texture> exhustTexture = scene->engine->Get<PhoenixEngine::ResourceSystem>()->Get<PhoenixEngine::Texture>("sprites/exhust.png", scene->engine->Get<PhoenixEngine::Renderer>());
	scene->engine->Get<PhoenixEngine::ParticleSystem>()->Create(children[0]->transform.position, 3, 2, exhustTexture, 50, children[0]->transform.rotation, PhoenixEngine::DegToRad(30));
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
			std::shared_ptr<PhoenixEngine::Texture> playerParticle = scene->engine->Get<PhoenixEngine::ResourceSystem>()->Get<PhoenixEngine::Texture>("sprites/playerpart.png", scene->engine->Get<PhoenixEngine::Renderer>());
			scene->engine->Get<PhoenixEngine::ParticleSystem>()->Create(children[0]->transform.position, 10, 2, playerParticle, 50, children[0]->transform.rotation, PhoenixEngine::DegToRad(30));
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

	if (dynamic_cast<Enemy*>(actor) || dynamic_cast<Asteroid*>(actor))
	{
		health = 0;
		destroy = true;
		PhoenixEngine::SetFilePath("../../Build/Resources");
		std::shared_ptr<PhoenixEngine::Texture> playerParticle = scene->engine->Get<PhoenixEngine::ResourceSystem>()->Get<PhoenixEngine::Texture>("sprites/playerpart.png", scene->engine->Get<PhoenixEngine::Renderer>());
		scene->engine->Get<PhoenixEngine::ParticleSystem>()->Create(children[0]->transform.position, 10, 2, playerParticle, 50, children[0]->transform.rotation, PhoenixEngine::DegToRad(30));
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

