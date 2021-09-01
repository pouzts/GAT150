#pragma once
#include "Component/Component.h"
#include "Framework/EventSystem.h"

class ProjectileComponent : public PhoenixEngine::Component
{
public:
	virtual ~ProjectileComponent();

	std::unique_ptr<Object> Clone() const { return std::make_unique<ProjectileComponent>(*this); }

	void Create() override;
	virtual void Update() override;

	virtual void OnCollisionEnter(const PhoenixEngine::Event& event);

	virtual bool Write(const rapidjson::Value& value) const override;
	virtual bool Read(const rapidjson::Value& value) override;

public:
	float speed{ 0 };
	float lifeTime{ 0 };
	int damage{0};

private:
	std::list<PhoenixEngine::Actor*> contacts;
};