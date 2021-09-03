#pragma once
#include "Engine.h"
#include "Graphics/Renderer.h"

class Game
{
public:
	enum class eState
	{
		Reset,
		Title,
		StartGame,
		StartLevel,
		Level,
		PlayerDead,
		GameOver
	};

public:
	void Initialize();
	void Shutdown();

	void Update();
	void Draw();

	bool HasQuit() { return quit; }

private:
	void Reset();
	void Title();
	void StartGame();
	void StartLevel();
	void Level();
	void PlayerDead();
	void GameOver();

	void OnAddScore(const PhoenixEngine::Event event);

public:
	std::unique_ptr<PhoenixEngine::Engine> engine;
	std::unique_ptr<PhoenixEngine::Scene> scene;

private:
	eState state = eState::Reset;
	
	bool quit = false;
	
	float stateTimer = 0;
	float spawnTimer = 0;
	
	int score = 0;
	int highScore = 0;
};
