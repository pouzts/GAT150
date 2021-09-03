#include "EnemyComponent.h"
#include "Engine.h"

using namespace PhoenixEngine;

EnemyComponent::~EnemyComponent()
{
	owner->scene->engine->Get<EventSystem>()->Unsubscribe("collision_enter", owner);
}

void EnemyComponent::Create()
{
	owner->scene->engine->Get<EventSystem>()->Subscribe("collision_enter", std::bind(&EnemyComponent::OnCollisionEnter, this, std::placeholders::_1), owner);

	owner->scene->engine->Get<AudioSystem>()->AddAudio("squeak", "audio/squeak.wav");
}

void EnemyComponent::Update()
{
	if (health <= 0)
	{
		auto coin = PhoenixEngine::ObjectFactory::Instance().Create<PhoenixEngine::Actor>("Coin");
		coin->transform.position = PhoenixEngine::Vector2{ owner->transform.position.x, owner->transform.position.y };
		owner->scene->AddActor(std::move(coin));
	}

	owner->destroy = (health <= 0);

	Actor* player = owner->scene->FindActor("Player");
	if (player)
	{
		Vector2 direction = player->transform.position - owner->transform.position;
		Vector2 force = direction.Normalized() * speed;

		PhysicsComponent* physicsComponent = owner->GetComponent<PhysicsComponent>();
		assert(physicsComponent);

		physicsComponent->ApplyForce(force);
	}
}

void EnemyComponent::OnCollisionEnter(const PhoenixEngine::Event& event)
{
	void* p = std::get<void*>(event.data);
	Actor* actor = reinterpret_cast<Actor*>(p);

	if (istring_compare(actor->tag, "projectile"))
	{
		owner->scene->engine->Get<AudioSystem>()->PlayAudio("squeak");
		health--;
	}
}

void EnemyComponent::OnCollisionExit(const PhoenixEngine::Event& event)
{
}


bool EnemyComponent::Write(const rapidjson::Value& value) const
{
	return false;
}

bool EnemyComponent::Read(const rapidjson::Value& value)
{
	JSON_READ(value, speed);
	JSON_READ(value, health);

	return true;
}