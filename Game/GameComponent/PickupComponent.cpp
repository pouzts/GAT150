#include "PickupComponent.h"
#include "Engine.h"

using namespace PhoenixEngine;

PickupComponent::~PickupComponent()
{
	owner->scene->engine->Get<EventSystem>()->Unsubscribe("collision_enter", owner);
	owner->scene->engine->Get<EventSystem>()->Unsubscribe("collision_exit", owner);
}

void PickupComponent::Create()
{
	owner->scene->engine->Get<EventSystem>()->Subscribe("collision_enter", std::bind(&PickupComponent::OnCollisionEnter, this, std::placeholders::_1), owner);
	owner->scene->engine->Get<EventSystem>()->Subscribe("collision_exit", std::bind(&PickupComponent::OnCollisionEnter, this, std::placeholders::_1), owner);

	owner->scene->engine->Get<AudioSystem>()->AddAudio("coin", "audio/coin.wav");
}

void PickupComponent::Update()
{
	
}

void PickupComponent::OnCollisionEnter(const PhoenixEngine::Event& event)
{
	void* p = std::get<void*>(event.data);
	Actor* actor = reinterpret_cast<Actor*>(p);

	if (istring_compare(actor->tag, "ground"))
	{
		contacts.push_back(actor);
	}

	if (istring_compare(actor->tag, "player"))
	{
		owner->destroy = true;
		owner->scene->engine->Get<AudioSystem>()->PlayAudio("coin");
	}
}

bool PickupComponent::Write(const rapidjson::Value& value) const
{
	return false;
}

bool PickupComponent::Read(const rapidjson::Value& value)
{
	JSON_READ(value, points);

	return true;
}
