#pragma once
#include "Component/Component.h"

class EnemyComponent : public PhoenixEngine::Component
{
public:
	std::unique_ptr<Object> Clone() const { return std::make_unique<EnemyComponent>(*this); }
	virtual bool Write(const rapidjson::Value& value) const override;
	virtual bool Read(const rapidjson::Value& value) override;
	virtual void Update() override;
public:
	float speed{ 0 };
	int health{0};
};