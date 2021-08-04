#include "Graphics/Texture.h"
#include "Graphics/Renderer.h"
#include <SDL_Image.h>
#include <iostream>

namespace PhoenixEngine
{
	bool Texture::Load(const std::string& filename, void* data)
	{
		renderer = static_cast<Renderer*>(data)->renderer;

		SDL_Surface* surface = IMG_Load(filename.c_str());
		if (surface == nullptr)
		{
			std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
			return false;
		}

		texture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);
		if (texture == nullptr)
		{
			std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
			return false;
		}
		return true;
	}
}