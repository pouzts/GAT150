#pragma once
#include "Engine.h"
#include "Graphics/Renderer.h"

class Game
{
public:
	enum class eState 
	{
		Title,
		StartGame,
		StartLevel,
		Game,
		EndLevel,
		GameOver
	};

public:
	void Initialize();
	void Shutdown();

	void Update();
	void Draw();

	bool HasQuit() { return quit; }
private:
	void UpdateTitle(float dt);
	void UpdateLevelStart(float dt);
	
	void OnAddPoints(const PhoenixEngine::Event& event);
	void OnPlayerDead(const PhoenixEngine::Event& event);

public:
	std::unique_ptr<PhoenixEngine::Engine> engine;
	std::unique_ptr<PhoenixEngine::Scene> scene;

private:
	bool quit = false;
	eState state = eState::Title;
	float stateTimer = 0.0f;

	size_t score = 0;
	size_t health = 0;
	size_t level = 1;
	
	PhoenixEngine::AudioChannel musicChannel;
	
	// Actor Textures
	std::shared_ptr<PhoenixEngine::Texture> playerTexture;
	std::shared_ptr<PhoenixEngine::Texture> enemyTexture;
	std::shared_ptr<PhoenixEngine::Texture> asteroidTexture;

	// Text Textures
	std::shared_ptr<PhoenixEngine::Texture> textTexture;
	std::shared_ptr<PhoenixEngine::Texture> scoreTexture;
	std::shared_ptr<PhoenixEngine::Texture> healthTexture;

	std::shared_ptr<PhoenixEngine::Font> font;
};

