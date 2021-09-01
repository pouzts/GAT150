#include "PlayerComponent.h"
#include "Engine.h"

using namespace PhoenixEngine;

PlayerComponent::~PlayerComponent()
{
	owner->scene->engine->Get<EventSystem>()->Unsubscribe("collision_enter", owner);
}

void PlayerComponent::Create()
{
	owner->scene->engine->Get<EventSystem>()->Subscribe("collision_enter", std::bind(&PlayerComponent::OnCollisionEnter, this, std::placeholders::_1), owner);

	owner->scene->engine->Get<AudioSystem>()->AddAudio("hurt", "audio/hurt.wav");
}

void PlayerComponent::Update()
{
	owner->destroy = (health <= 0);

	// Movement
	Vector2 force = Vector2::zero;
	if (owner->scene->engine->Get<InputSystem>()->GetKeyState(SDL_SCANCODE_LEFT) == InputSystem::eKeyState::Held)
	{
		force.x -= speed;
	}

	if (owner->scene->engine->Get<InputSystem>()->GetKeyState(SDL_SCANCODE_RIGHT) == InputSystem::eKeyState::Held)
	{
		force.x += speed;
	}

	if (contacts.size() > 0 && owner->scene->engine->Get<InputSystem>()->GetKeyState(SDL_SCANCODE_UP) == InputSystem::eKeyState::Pressed)
	{
		force.y -= jumpForce;
	}

	PhysicsComponent* physicsComponent = owner->GetComponent<PhysicsComponent>();
	assert(physicsComponent);

	physicsComponent->ApplyForce(force);
	
	// Changes Sprite Animation based on direction.
	SpriteAnimationComponent* spriteAnimationComponent = owner->GetComponent<SpriteAnimationComponent>();
	assert(spriteAnimationComponent);
	if (physicsComponent->velocity.x > 0) spriteAnimationComponent->StartSequence("walk_right");
	else if (physicsComponent->velocity.x < 0) spriteAnimationComponent->StartSequence("walk_left");
	else spriteAnimationComponent->StartSequence("idle");
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

