#pragma once
#include "Base/Actor.h"

class Projectile : public PhoenixEngine::Actor
{
public:
	Projectile(const PhoenixEngine::Transform& transform, std::shared_ptr<PhoenixEngine::Shape> shape, float speed) : Actor{ transform, shape }, speed{ speed } {}

	void Update(float dt) override;

private:
	float lifetime{ 2 };
	float speed{ 300 };
};
