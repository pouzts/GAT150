#include "Actor.h"
#include "Graphics/Renderer.h"
#include "Component/GraphicComponent.h"
#include <algorithm>

namespace PhoenixEngine
{
	void Actor::Update(float dt)
	{
		std::for_each(components.begin(), components.end(), [](auto& component) { component->Update(); });
		transform.Update();
		std::for_each(children.begin(), children.end(), [](auto& child) {child->transform.Update(child->parent->transform.matrix); });
	}

	void Actor::Draw(Renderer* renderer)
	{
		std::for_each(components.begin(), components.end(), [renderer](auto& component) {
			if (dynamic_cast<GraphicComponent*>(component.get()))
			{
				dynamic_cast<GraphicComponent*>(component.get())->Draw(renderer);
			}
		});
		std::for_each(children.begin(), children.end(), [renderer](auto& child) {child->Draw(renderer); });
	}

	void Actor::AddChild(std::unique_ptr<Actor> child)
	{
		child->parent = this;
		children.push_back(std::move(child));
	}

	void Actor::AddComponent(std::unique_ptr<Component> component)
	{
		component->owner = this;
		components.push_back(std::move(component));
	}

	float Actor::GetRadius()
	{
		return 0.0f;
	}
}