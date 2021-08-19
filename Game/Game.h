#pragma once
#include "Engine.h"
#include "Graphics/Renderer.h"

class Game
{
public:
	void Initialize();
	void Shutdown();

	void Update();
	void Draw();

	bool HasQuit() { return quit; }
public:
	std::unique_ptr<PhoenixEngine::Engine> engine;
	std::unique_ptr<PhoenixEngine::Scene> scene;

private:
	bool quit = false;
};
