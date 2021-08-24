#include "Actor.h"
#include "Engine.h"
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
	
	bool Actor::Write(const rapidjson::Value& value) const
	{
		return false;
	}
	
	bool Actor::Read(const rapidjson::Value& value)
	{
		JSON_READ(value, tag);
		if (value.HasMember("transform"))
		{
			transform.Read(value["transform"]);
		}

		if (value.HasMember("components") && value["components"].IsArray())
		{
			for (auto& componentValue : value["components"].GetArray())
			{
				std::string type;
				JSON_READ(componentValue, type);

				auto component = ObjectFactory::Instance().Create<Component>(type);
				if (component)
				{
					component->owner = this;
					component->Read(componentValue);
					AddComponent(std::move(component));
				}
			}
		}

		return true;
	}
}