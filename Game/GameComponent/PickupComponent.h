#pragma once
#include "Component/Component.h"
#include "Framework/EventSystem.h"

class PickupComponent : public PhoenixEngine::Component
{
public:
	virtual ~PickupComponent();

	std::unique_ptr<Object> Clone() const { return std::make_unique<PickupComponent>(*this); }
	
	void Create() override;
	virtual void Update() override;

	virtual void OnCollisionEnter(const PhoenixEngine::Event& event);

	virtual bool Write(const rapidjson::Value& value) const override;
	virtual bool Read(const rapidjson::Value& value) override;

public:
	int points{0};

private:
	std::list<PhoenixEngine::Actor*> contacts;
};