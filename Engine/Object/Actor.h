#pragma once
#include "Object/Object.h"
#include "Math/Transform.h"
#include <memory>
#include <vector>

namespace PhoenixEngine
{
	class Shape;
	class Scene;

	class Actor : public Object
	{
	public:
		Actor() {}
		Actor(const Transform& transform, std::shared_ptr<Shape> shape = {}) : transform{ transform }, shape{ shape } {}

		virtual void Initialize();

		virtual void Update(float dt);
		virtual void Draw();

		virtual void OnCollision(Actor* actor) {}

		void AddChild(std::unique_ptr<Actor> child);

		float GetRadius();
	
	public:
		bool destroy = false;
		std::string tag;

		Transform transform;
		std::shared_ptr<Shape> shape;
		Scene* scene{nullptr};

		Actor* parent{ nullptr };
		std::vector<std::unique_ptr<Actor>> children;
	};
}