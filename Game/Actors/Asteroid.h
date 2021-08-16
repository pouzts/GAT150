#pragma once
#include "Object/Actor.h"

class Asteroid : public PhoenixEngine::Actor
{
public:
	Asteroid(const PhoenixEngine::Transform& transform, std::shared_ptr<PhoenixEngine::Texture> texture, float speed) : Actor{ transform, texture }, speed{ speed } {}

	void Update(float dt) override;
	void OnCollision(Actor* actor) override;

private:
	float speed{ 150 };
};