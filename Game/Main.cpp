#include "Engine.h"
#include <SDL.h>
#include <SDL_Image.h>
#include <iostream>

int main(int, char**)
{
	PhoenixEngine::Engine engine;
	engine.Startup();
	engine.Get<PhoenixEngine::Renderer>()->Create("GAT150", 800, 600);

	PhoenixEngine::Scene scene;

	scene.engine = &engine;

	PhoenixEngine::SetFilePath("../Resources");

	std::shared_ptr<PhoenixEngine::Texture> texture = engine.Get<PhoenixEngine::ResourceSystem>()->Get<PhoenixEngine::Texture>("sf2.png", engine.Get<PhoenixEngine::Renderer>());

	for (int i = 0; i < 200; i++)
	{
		PhoenixEngine::Transform transform{ PhoenixEngine::Vector2{PhoenixEngine::RandomRange(0.0f, 800.0f), PhoenixEngine::RandomRange(0.0f, 600.0f)}, PhoenixEngine::RandomRange(0.0f, 360.0f), 1.0f };
		std::unique_ptr<PhoenixEngine::Actor> actor = std::make_unique<PhoenixEngine::Actor>(transform, texture);

		scene.AddActor(std::move(actor));
	}


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

		engine.Update(0);
		scene.Update(0);

		engine.Get<PhoenixEngine::Renderer>()->BeginFrame();
		
		scene.Draw(engine.Get<PhoenixEngine::Renderer>());

		engine.Get<PhoenixEngine::Renderer>()->EndFrame();
	}

	SDL_Quit();

	return 0;
}
