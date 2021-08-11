#pragma once
#include "Resource/Resource.h"
#include "Math/Vector2.h"
#include <SDL.h>

namespace PhoenixEngine{

	class Renderer;

	class Texture : public Resource
	{
	public:
		Texture() {}
		Texture(Renderer* renderer);

		bool Load(const std::string& filename, void* data) override;
		bool Create(SDL_Surface* surface);

		Vector2 GetSize() const;

		friend class Renderer;
	
	private:
		SDL_Texture* texture{ nullptr };
		SDL_Renderer* renderer{ nullptr };
	};
}