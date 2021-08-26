#pragma once
#include "SpriteComponent.h"
#include <SDL.h>

namespace PhoenixEngine
{
	class SpriteAnimationComponent : public SpriteComponent
	{
	public:
		void Update() override;
		void Draw(Renderer* renderer) override;
		// Inherited via SpriteComponent
		virtual bool Write(const rapidjson::Value& value) const override;
		virtual bool Read(const rapidjson::Value& value) override;
	
	public:
		int frame{ 0 }, fps{ 0 }, numFramesX{ 0 }, numFramesY{ 0 }, startFrame{0}, endFrame{0};
		float frameTimer{ 0.0f }, frameTime{ 0.0f };
		SDL_Rect rect;
	};
}