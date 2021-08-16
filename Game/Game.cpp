#include "Game.h"
#include "Object/Actor.h"
#include "Actors/Player.h"
#include "Actors/Enemy.h"
#include "Actors/Asteroid.h"
#include "Resource/ResourceSystem.h"

void Game::Initialize()
{
	engine = std::make_unique<PhoenixEngine::Engine>();
	engine->Startup();
	engine->Get<PhoenixEngine::Renderer>()->Create("GAT150", 800, 600);

	scene = std::make_unique<PhoenixEngine::Scene>();
	scene->engine = engine.get();

	PhoenixEngine::SetFilePath("../Resources");

	engine->Get<PhoenixEngine::AudioSystem>()->AddAudio("explosion", "audio/explosion.wav");
	engine->Get<PhoenixEngine::AudioSystem>()->AddAudio("music", "audio/music.mp3");
	musicChannel = engine->Get<PhoenixEngine::AudioSystem>()->PlayAudio("music", 1, 1, true);

	// Create actor textures
	playerTexture = engine->Get<PhoenixEngine::ResourceSystem>()->Get<PhoenixEngine::Texture>("sprites/player.png", engine->Get<PhoenixEngine::Renderer>());
	enemyTexture = engine->Get<PhoenixEngine::ResourceSystem>()->Get<PhoenixEngine::Texture>("sprites/enemy.png", engine->Get<PhoenixEngine::Renderer>());
	asteroidTexture = engine->Get<PhoenixEngine::ResourceSystem>()->Get<PhoenixEngine::Texture>("sprites/asteroid.png", engine->Get<PhoenixEngine::Renderer>());

	// get font from resource system
	int size = 16;
	font = engine->Get<PhoenixEngine::ResourceSystem>()->Get<PhoenixEngine::Font>("fonts/November.ttf", &size);
	
	// create font texture
	textTexture = std::make_shared<PhoenixEngine::Texture>(engine->Get<PhoenixEngine::Renderer>());
	// set font texture with font surface
	textTexture->Create(font->CreateSurface("HELLO WORLD", PhoenixEngine::Color::white));
	// add font texture to resource system
	engine->Get<PhoenixEngine::ResourceSystem>()->Add("textTexture", textTexture);

	// Score Texture
	scoreTexture = std::make_shared<PhoenixEngine::Texture>(engine->Get<PhoenixEngine::Renderer>());
	// set font texture with font surface
	scoreTexture->Create(font->CreateSurface("HELLO WORLD", PhoenixEngine::Color::white));
	// add font texture to resource system
	engine->Get<PhoenixEngine::ResourceSystem>()->Add("scoreTexture", scoreTexture);

	healthTexture = std::make_shared<PhoenixEngine::Texture>(engine->Get<PhoenixEngine::Renderer>());
	// set font texture with font surface
	healthTexture->Create(font->CreateSurface("HELLO WORLD", PhoenixEngine::Color::white));
	// add font texture to resource system
	engine->Get<PhoenixEngine::ResourceSystem>()->Add("healthTexture", healthTexture);

	engine->Get<PhoenixEngine::AudioSystem>()->AddAudio("playerFire", "audio/player_fire.wav");
	engine->Get<PhoenixEngine::AudioSystem>()->AddAudio("enemyFire", "audio/enemy_fire.wav");

	engine->Get<PhoenixEngine::AudioSystem>()->AddAudio("explosion", "audio/explosion.wav");
	engine->Get<PhoenixEngine::AudioSystem>()->AddAudio("hurt", "audio/hurt.wav");

	engine->Get<PhoenixEngine::EventSystem>()->Subscribe("AddPoints", std::bind(&Game::OnAddPoints, this, std::placeholders::_1));
	engine->Get<PhoenixEngine::EventSystem>()->Subscribe("PlayerDead", std::bind(&Game::OnPlayerDead, this, std::placeholders::_1));
}

void Game::Shutdown()
{
	scene->RemoveAllActors();
	engine->Shutdown();
}

void Game::Update()
{
	engine->Update();
	float dt = engine->time.deltaTime;

	stateTimer += dt;

	switch (state)
	{
	case Game::eState::Title:
		UpdateTitle(dt);
		break;
	case Game::eState::StartGame:
		score = 0;
		level = 1;
		state = eState::StartLevel;
		break;
	case Game::eState::StartLevel:
		UpdateLevelStart(dt);
		break;
	case Game::eState::Game:
		health = scene->GetActor<Player>()->GetHealth();
		if (scene->GetActors<Asteroid>().size() == 0 && scene->GetActors<Enemy>().size() == 0)
		{
			state = eState::EndLevel;
			stateTimer = 0;
			level++;
		}
		break;
	case Game::eState::EndLevel:
		if (stateTimer >= 5)
		{
			state = eState::StartLevel;
			scene->RemoveAllActors();
		}
		break;
	case Game::eState::GameOver:
		if (stateTimer >= 5)
		{
			scene->RemoveAllActors();
			state = eState::Title;
		}
		break;
	default:
		break;
	}


	if (engine->Get<PhoenixEngine::InputSystem>()->GetKeyState(SDL_SCANCODE_ESCAPE) == PhoenixEngine::InputSystem::eKeyState::Pressed)
	{
		quit = true;
	}

	engine->time.timeScale = 2;
	scene->Update(engine->time.deltaTime);
}

void Game::Draw()
{
	std::string scoreText = "SCORE: " + std::to_string(score);
	std::string healthText = "HEALTH: " + std::to_string(health);
	std::string updateText = "    ";

	switch (state)
	{
	case Game::eState::Title:
		updateText = "Press SPACE to Continue";
		break;
	case Game::eState::StartGame:
		updateText = "     ";
		break;
	case Game::eState::StartLevel:
		updateText = "     ";
		break;
	case Game::eState::Game:
		break;
	case Game::eState::EndLevel:
	{
		updateText = "Level " + std::to_string(level);
		//textTexture->Create(font->CreateSurface(levelname, PhoenixEngine::Color::white));
	}
	break;
	case Game::eState::GameOver:
		updateText = "Game Over";
		break;
	default:
		break;
	}

	// draw
	engine->Get<PhoenixEngine::Renderer>()->BeginFrame();

	engine->Draw(engine->Get<PhoenixEngine::Renderer>());
	scene->Draw(engine->Get<PhoenixEngine::Renderer>());

	//Update Text
	engine->Get<PhoenixEngine::ResourceSystem>()->Get<PhoenixEngine::Texture>("textTexture");
	textTexture->Create(font->CreateSurface(updateText, PhoenixEngine::Color::white));
	//Update score
	engine->Get<PhoenixEngine::ResourceSystem>()->Get<PhoenixEngine::Texture>("scoreTexture");
	scoreTexture->Create(font->CreateSurface(scoreText, PhoenixEngine::Color::white));
	//Update health
	engine->Get<PhoenixEngine::ResourceSystem>()->Get<PhoenixEngine::Texture>("textTexture");
	healthTexture->Create(font->CreateSurface(healthText, PhoenixEngine::Color::white));
	{
		PhoenixEngine::Transform t;
		t.position = { 400, 400 };
		engine->Get<PhoenixEngine::Renderer>()->Draw(textTexture, t);
	}

	{
		PhoenixEngine::Transform t;
		t.position = { 60, 30 };
		engine->Get<PhoenixEngine::Renderer>()->Draw(scoreTexture, t);
	}

	{
		PhoenixEngine::Transform t;
		t.position = { 740, 30 };
		engine->Get<PhoenixEngine::Renderer>()->Draw(healthTexture, t);
	}

	engine->Get<PhoenixEngine::Renderer>()->EndFrame();
}

void Game::UpdateTitle(float dt)
{
	if (engine->Get<PhoenixEngine::InputSystem>()->GetKeyState(SDL_SCANCODE_SPACE) == PhoenixEngine::InputSystem::eKeyState::Pressed) {
		state = eState::StartGame;
	}
}

void Game::UpdateLevelStart(float dt)
{
	scene->AddActor(std::make_unique<Player>(PhoenixEngine::Transform{ PhoenixEngine::Vector2{400, 300}, 0, 3 }, playerTexture, 150.0f));

	for (size_t i = 0; i < level; i++)
	{
		bool hasSpawned = false;
		while (!hasSpawned) {
			float spawnX = PhoenixEngine::RandomRange(0.0f, 800.0f);
			float spawnY = PhoenixEngine::RandomRange(0.0f, 600.0f);
			if ((spawnX < 300.0f || spawnX > 500.0f) && (spawnY < 200.0f || spawnY > 400.0f)) 
			{
				scene->AddActor(std::make_unique<Enemy>(PhoenixEngine::Transform{ PhoenixEngine::Vector2{spawnX, spawnY}, PhoenixEngine::RandomRange(0.0f, PhoenixEngine::TwoPi), 3 }, enemyTexture , 150.0f));
				hasSpawned = true;
			}
		}
	}

	for (size_t i = 0; i < 2 + level; i++)
	{
		bool hasSpawned = false;
		while (!hasSpawned) {
			float spawnX = PhoenixEngine::RandomRange(0.0f, 800.0f);
			float spawnY = PhoenixEngine::RandomRange(0.0f, 600.0f);
			if ((spawnX < 300.0f || spawnX > 500.0f) && (spawnY < 200.0f || spawnY > 400.0f))
			{
				scene->AddActor(std::make_unique<Asteroid>(PhoenixEngine::Transform{ PhoenixEngine::Vector2{spawnX, spawnY}, PhoenixEngine::RandomRange(0.0f, PhoenixEngine::TwoPi), 3 }, asteroidTexture, 150.0f));
				hasSpawned = true;
			}
		}
	}

	state = eState::Game;
}

void Game::OnAddPoints(const PhoenixEngine::Event& event)
{
	score += std::get<int>(event.data);
}

void Game::OnPlayerDead(const PhoenixEngine::Event& event)
{
	std::cout << std::get<std::string>(event.data) << std::endl;
	stateTimer = 0;
	state = eState::GameOver;
}
