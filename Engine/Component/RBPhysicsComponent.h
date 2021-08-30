#pragma once
#include "PhysicsComponent.h"
#include "Physics/PhysicsSystem.h"

namespace PhoenixEngine
{
	class RBPhysicsComponent : public PhysicsComponent
	{
	public:
		std::unique_ptr<Object> Clone() const { return std::make_unique<RBPhysicsComponent>(*this); }

		virtual ~RBPhysicsComponent();

		void Update() override;
		void ApplyForce(const Vector2& force) override;
		// Inherited via Component
		virtual bool Write(const rapidjson::Value& value) const override;
		virtual bool Read(const rapidjson::Value& value) override;

	public:
		PhysicsSystem::RigidBodyData data;
		b2Body* body{nullptr};
	};
}