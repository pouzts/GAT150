#pragma once
#include "GraphicComponent.h"
#include <memory>

namespace PhoenixEngine
{
	class Texture;

	class SpriteComponent : public GraphicComponent
	{
	public:
		void Update() override;
		void Draw(Renderer* renderer) override;
	
		// Inherited via GraphicComponent
		virtual bool Write(const rapidjson::Value& value) const override;
		virtual bool Read(const rapidjson::Value& value) override;
	public:
		std::shared_ptr<Texture> texture;
	};
}