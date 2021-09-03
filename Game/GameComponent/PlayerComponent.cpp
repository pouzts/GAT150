#include "PlayerComponent.h"
#include "ProjectileComponent.h"
#include "Engine.h"

using namespace PhoenixEngine;

PlayerComponent::~PlayerComponent()
{
	owner->scene->engine->Get<EventSystem>()->Unsubscribe("collision_enter", owner);
	owner->scene->engine->Get<EventSystem>()->Unsubscribe("collision_exit", owner);
}

void PlayerComponent::Create()
{
	owner->scene->engine->Get<EventSystem>()->Subscribe("collision_enter", std::bind(&PlayerComponent::OnCollisionEnter, this, std::placeholders::_1), owner);
	owner->scene->engine->Get<EventSystem>()->Subscribe("collision_exit", std::bind(&PlayerComponent::OnCollisionExit, this, std::placeholders::_1), owner);

	owner->scene->engine->Get<AudioSystem>()->AddAudio("hurt", "audio/hurt.wav");
	owner->scene->engine->Get<AudioSystem>()->AddAudio("laser", "audio/player_fire.wav");
}

void PlayerComponent::Update()
{
	owner->destroy = (health <= 0);

	// Movement
	Vector2 force = Vector2::zero;
	if (owner->scene->engine->Get<InputSystem>()->GetKeyState(SDL_SCANCODE_A) == InputSystem::eKeyState::Held)
	{
		dir = eDirection::Left;
		force.x -= speed;
	}
	
	if (owner->scene->engine->Get<InputSystem>()->GetKeyState(SDL_SCANCODE_D) == InputSystem::eKeyState::Held)
	{
		dir = eDirection::Right;
		force.x += speed;
	}

	if (owner->scene->engine->Get<InputSystem>()->GetKeyState(SDL_SCANCODE_W) == InputSystem::eKeyState::Held)
	{
		dir = eDirection::Up;
	}

	if (contacts.size() > 0 && owner->scene->engine->Get<InputSystem>()->GetKeyState(SDL_SCANCODE_SPACE) == InputSystem::eKeyState::Pressed)
	{
		force.y -= jumpForce;
	}

	PhysicsComponent* physicsComponent = owner->GetComponent<PhysicsComponent>();
	assert(physicsComponent);

	physicsComponent->ApplyForce(force);
	
	// Changes Sprite Animation based on direction.
	SpriteAnimationComponent* spriteAnimationComponent = owner->GetComponent<SpriteAnimationComponent>();
	assert(spriteAnimationComponent);
	if (dir == eDirection::Right) spriteAnimationComponent->StartSequence("walk_right");
	else if (dir == eDirection::Left) spriteAnimationComponent->StartSequence("walk_left");
	else spriteAnimationComponent->StartSequence("shoot_up");

	//	Fires projectile
	if (owner->scene->engine->Get<InputSystem>()->GetButtonState(0) == InputSystem::eKeyState::Pressed)
	{
		owner->scene->engine->Get<AudioSystem>()->PlayAudio("laser");

		auto projectile = PhoenixEngine::ObjectFactory::Instance().Create<PhoenixEngine::Actor>("Projectile");
		projectile->transform.position = owner->transform.position;
		owner->scene->AddActor(std::move(projectile));
	}
}

void PlayerComponent::OnCollisionEnter(const PhoenixEngine::Event& event)
{
	void* p = std::get<void*>(event.data);
	Actor* actor = reinterpret_cast<Actor*>(p);
	
	if (istring_compare(actor->tag, "ground"))
	{
		contacts.push_back(actor);
	}

	if (istring_compare(actor->tag, "enemy"))
	{
		owner->scene->engine->Get<AudioSystem>()->PlayAudio("hurt");
		health--;
	}

	std::cout << actor->tag << std::endl;
}

void PlayerComponent::OnCollisionExit(const PhoenixEngine::Event& event)
{
	void* p = std::get<void*>(event.data);
	Actor* actor = reinterpret_cast<Actor*>(p);

	if (istring_compare(actor->tag, "ground"))
	{
		contacts.remove(actor);
	}
}

bool PlayerComponent::Write(const rapidjson::Value& value) const
{
	return false;
}

bool PlayerComponent::Read(const rapidjson::Value& value)
{
	JSON_READ(value, speed);
	JSON_READ(value, jumpForce);
	JSON_READ(value, health);

	return true;
}

