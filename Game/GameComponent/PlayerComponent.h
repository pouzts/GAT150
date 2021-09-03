#pragma once
#include "Component/Component.h"
#include "Framework/EventSystem.h"

class PlayerComponent : public PhoenixEngine::Component
{
public:
	enum class eDirection
	{
		Up,
		Left,
		Right
	};
public:
	virtual ~PlayerComponent();

	std::unique_ptr<Object> Clone() const { return std::make_unique<PlayerComponent>(*this); }

	void Create() override;
	virtual void Update() override;

	virtual void OnCollisionEnter(const PhoenixEngine::Event& event);
	virtual void OnCollisionExit(const PhoenixEngine::Event& event);

	virtual bool Write(const rapidjson::Value& value) const override;
	virtual bool Read(const rapidjson::Value& value) override;

public:
	float speed{ 0 };
	float jumpForce{ 0 };
	int health{ 0 };

	eDirection dir = eDirection::Right;

private:
	std::list<PhoenixEngine::Actor*> contacts;
};