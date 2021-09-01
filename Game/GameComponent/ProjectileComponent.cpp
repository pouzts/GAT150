#include "ProjectileComponent.h"
#include "Engine.h"

using namespace PhoenixEngine;

ProjectileComponent::~ProjectileComponent()
{
	owner->scene->engine->Get<EventSystem>()->Unsubscribe("collision_enter", owner);

}

void ProjectileComponent::Create()
{
	owner->scene->engine->Get<EventSystem>()->Subscribe("collision_enter", std::bind(&ProjectileComponent::OnCollisionEnter, this, std::placeholders::_1), owner);

	owner->scene->engine->Get<AudioSystem>()->AddAudio("coin", "audio/coin.wav");
}

void ProjectileComponent::Update()
{
	float dt = owner->scene->engine->time.deltaTime;

	lifeTime -= dt;

	owner->destroy = (lifeTime <= 0);
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