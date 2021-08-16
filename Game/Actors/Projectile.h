#pragma once
#include "Object/Actor.h"

class Projectile : public PhoenixEngine::Actor
{
public:
	Projectile(const PhoenixEngine::Transform& transform, std::shared_ptr<PhoenixEngine::Texture> texture, float speed) : Actor{ transform, texture }, speed{ speed } {}

	void Update(float dt) override;

private:
	float lifetime{ 2 };
	float speed{ 300 };
};
