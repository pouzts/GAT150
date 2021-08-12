#pragma once
#include "Base/Actor.h"

class Astroid : public PhoenixEngine::Actor
{
public:
	Astroid(const PhoenixEngine::Transform& transform, std::shared_ptr<PhoenixEngine::Shape> shape, float speed) : Actor{ transform, shape }, speed{ speed } {}

	void Update(float dt) override;
	void OnCollision(Actor* actor) override;

private:
	float speed{ 150 };
};