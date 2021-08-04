#pragma once
#include "Framework/System.h"
#include "Graphics/Texture.h"
#include "Math/Vector2.h"
#include <SDL.h>
#include <string>

namespace PhoenixEngine
{
	class Renderer : public System
	{
	public:
		void Startup() override;
		void Shutdown() override;
		void Update(float dt) override;

		void Create(const std::string& name, int width, int height);
		void BeginFrame();
		void EndFrame();

		void Draw(std::shared_ptr<PhoenixEngine::Texture> texture, const Vector2& position);

		friend class Texture;

	private:
		SDL_Renderer* renderer{ nullptr };
		SDL_Window* window{nullptr};
	};
}