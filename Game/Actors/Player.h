#pragma once
#include "Object/Actor.h"

class Player : public PhoenixEngine::Actor
{
public:
	Player(const PhoenixEngine::Transform& transform, std::shared_ptr<PhoenixEngine::Texture> texture, float speed) : Actor(transform, texture), speed{ speed } {}

	void Initialize() override;

	void Update(float dt) override;
	void OnCollision(Actor* actor) override;

	size_t GetHealth();
	void SetHealth(size_t health);

private:
	float fireTimer{ 0 };
	float fireRate{ 0.2f };
	float speed{ 150 };

	size_t health{10};
	
	PhoenixEngine::Vector2 velocity;
};