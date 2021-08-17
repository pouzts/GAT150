#pragma once
#include "Component.h"
#include "Math/Vector2.h"

namespace PhoenixEngine
{
	class PhysicsComponent : public Component
	{
	public:
		void Update() override;
		void ApplyForce(const Vector2& force) { acceleration += force; }
	private:
		Vector2 velocity;
		Vector2 acceleration;
		Vector2 force;
	};
}