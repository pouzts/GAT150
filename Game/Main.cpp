#include "Game.h"

int main(int, char**)
{
	Game game;
	game.Initialize();

	rapidjson::Document document;
	bool success = PhoenixEngine::json::Load("json.txt", document);
	assert(success);

	std::string str;
	PhoenixEngine::json::Get(document, "string", str);
	std::cout << str << std::endl;

	bool b;
	PhoenixEngine::json::Get(document, "boolean", b);
	std::cout << b << std::endl;

	int i1;
	PhoenixEngine::json::Get(document, "integer1", i1);
	std::cout << i1 << std::endl;

	int i2;
	PhoenixEngine::json::Get(document, "integer2", i2);
	std::cout << i2 << std::endl;

	float f;
	PhoenixEngine::json::Get(document, "float", f);
	std::cout << f << std::endl;

	PhoenixEngine::Vector2 v2;
	PhoenixEngine::json::Get(document, "vector2", v2);
	std::cout << v2 << std::endl;

	PhoenixEngine::Color color;
	PhoenixEngine::json::Get(document, "color", color);
	std::cout << color << std::endl;



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
