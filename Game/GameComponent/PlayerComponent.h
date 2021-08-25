#pragma once
#include "Component/Component.h"

class PlayerComponent : public PhoenixEngine::Component
{
public:
	virtual bool Write(const rapidjson::Value& value) const override;
	virtual bool Read(const rapidjson::Value& value) override;
	virtual void Update() override;
public:
	float speed{ 0 };
};