#include "Engine.h"
#include <SDL.h>
#include <SDL_Image.h>
#include <iostream>

int main(int, char**)
{
	PhoenixEngine::Engine engine;
	engine.Startup();
	engine.Get<PhoenixEngine::Renderer>()->Create("GAT150", 800, 600);

	std::cout << PhoenixEngine::GetFilePath() << std::endl;
	PhoenixEngine::SetFilePath("../Resources");
	std::cout << PhoenixEngine::GetFilePath() << std::endl;

	std::shared_ptr<PhoenixEngine::Texture> texture = engine.Get<PhoenixEngine::ResourceSystem>()->Get<PhoenixEngine::Texture>("sf2.png", engine.Get<PhoenixEngine::Renderer>());

	bool quit = false;
	SDL_Event event;
	while (!quit)
	{
		SDL_PollEvent(&event);
		switch (event.type)
		{
			case SDL_QUIT:
				quit = true;
				break;
		}

		engine.Get<PhoenixEngine::Renderer>()->BeginFrame();
		
		PhoenixEngine::Vector2 position{ 300,300 };
		engine.Get<PhoenixEngine::Renderer>()->Draw(texture, position);

		engine.Get<PhoenixEngine::Renderer>()->EndFrame();

		/*for (int i = 0; i < 50; i++)
		{
			SDL_Rect src{ 32, 64, 32, 64 };
			SDL_Rect dest{ PhoenixEngine::RandomRangeInt(0,screen.x), PhoenixEngine::RandomRangeInt(0,screen.y), 32, 48 };
			SDL_RenderCopy(renderer, texture, &src, &dest);
		}
		*/
	}

	SDL_Quit();

	return 0;
}
