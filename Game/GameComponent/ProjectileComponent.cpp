#include "ProjectileComponent.h"
#include "PlayerComponent.h"
#include "Engine.h"

using namespace PhoenixEngine;

ProjectileComponent::~ProjectileComponent()
{
	owner->scene->engine->Get<EventSystem>()->Unsubscribe("collision_enter", owner);

}

void ProjectileComponent::Create()
{
	owner->scene->engine->Get<EventSystem>()->Subscribe("collision_enter", std::bind(&ProjectileComponent::OnCollisionEnter, this, std::placeholders::_1), owner);
}

void ProjectileComponent::Update()
{
	float dt = owner->scene->engine->time.deltaTime;

	lifeTime -= dt;

	owner->destroy = (lifeTime <= 0);

	Vector2 force = Vector2::zero;
	PlayerComponent::eDirection dir = PlayerComponent::eDirection::Up;
	if (owner->scene->FindActor("Player")) dir = owner->scene->FindActor("Player")->GetComponent<PlayerComponent>()->dir;
	float rotation = 0.0f;

	if (dir == PlayerComponent::eDirection::Up) 
	{
		owner->transform.rotation = -90.0f;
		force.y -= speed; 
	}
	else if (dir == PlayerComponent::eDirection::Left) 
	{
		owner->transform.rotation = 180.0f;
		force.x -= speed; 
	}
	else
	{ 
		owner->transform.rotation = 0.0f;
		force.x += speed; 
	}

	PhysicsComponent* physicsComponent = owner->GetComponent<PhysicsComponent>();
	assert(physicsComponent);

	physicsComponent->ApplyForce(force);
}

void ProjectileComponent::OnCollisionEnter(const PhoenixEngine::Event& event)
{
	void* p = std::get<void*>(event.data);
	Actor* actor = reinterpret_cast<Actor*>(p);

	if (istring_compare(actor->tag, "enemy"))
	{
		owner->destroy = true;
	}
}

bool ProjectileComponent::Write(const rapidjson::Value& value) const
{
	return false;
}

bool ProjectileComponent::Read(const rapidjson::Value& value)
{
	JSON_READ(value, speed);
	JSON_READ(value, lifeTime);

	return true;
}