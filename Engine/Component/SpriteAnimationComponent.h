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
	public:
		int frame{ 0 }, fps{ 0 }, numFramesX{ 0 }, numFramesY{0};
		float frameTimer{ 0.0f }, frameTime{ 0.0f };
		SDL_Rect rect;
	};
}