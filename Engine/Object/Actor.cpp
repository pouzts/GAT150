#include "Actor.h"
#include <algorithm>

namespace PhoenixEngine
{
	void Actor::Initialize()
	{
	}
	void Actor::Update(float dt)
	{
		transform.Update();
		std::for_each(children.begin(), children.end(), [](auto& child) {child->transform.Update(child->parent->transform.matrix); });
	}

	void Actor::Draw()
	{
		
	}

	void Actor::AddChild(std::unique_ptr<Actor> child)
	{
		child->parent = this;
		children.push_back(std::move(child));
	}

	float Actor::GetRadius()
	{
		return 0;
	}
}