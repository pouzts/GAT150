#include "Game.h"

int main(int, char**)
{
	Game game;
	game.Initialize();

	rapidjson::Document document;
	bool success = PhoenixEngine::json::Load("json.txt", document);
	assert(success);

	bool quit = false;
	SDL_Event event;

	while (!quit && !game.HasQuit())
	{
		SDL_PollEvent(&event);
		switch (event.type)
		{
			case SDL_QUIT:
				quit = true;
				break;
		}
		game.Update();
		game.Draw();
	}

	SDL_Quit();

	return 0;
}
