#pragma once
#include "Component/Component.h"
#include "Framework/EventSystem.h"

class PickupComponent : public PhoenixEngine::Component
{
public:
	void Create() override;
	virtual void Update() override;

	virtual void OnCollisionEnter(const PhoenixEngine::Event& event);
	virtual void OnCollisionExit(const PhoenixEngine::Event& event);

	virtual bool Write(const rapidjson::Value& value) const override;
	virtual bool Read(const rapidjson::Value& value) override;

public:
	int points{0};

private:
	std::list<PhoenixEngine::Actor*> contacts;
};