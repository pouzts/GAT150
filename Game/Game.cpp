#include "Game.h"
#include "GameComponent/PlayerComponent.h"
#include "GameComponent/EnemyComponent.h"
#include "GameComponent/PickupComponent.h"
#include "GameComponent/ProjectileComponent.h"

void Game::Initialize()
{
	// create engine
	engine = std::make_unique<PhoenixEngine::Engine>();
	engine->Startup();
	engine->Get<PhoenixEngine::Renderer>()->Create("GAT150", 800, 600);

	// register classes
	REGISTER_CLASS(PlayerComponent);
	REGISTER_CLASS(EnemyComponent);
	REGISTER_CLASS(PickupComponent);
	REGISTER_CLASS(ProjectileComponent);

	// create scene
	scene = std::make_unique<PhoenixEngine::Scene>();
	scene->engine = engine.get();

	PhoenixEngine::SetFilePath("../Resources");
	
	scene->engine->Get<PhoenixEngine::AudioSystem>()->AddAudio("music", "audio/music.mp3");
	scene->engine->Get<PhoenixEngine::AudioSystem>()->PlayAudio("music",0.5f);

	engine->Get<PhoenixEngine::EventSystem>()->Subscribe("add_score", std::bind(&Game::OnAddScore, this, std::placeholders::_1));
}

void Game::Shutdown()
{
	scene->RemoveAllActors();
	engine->Shutdown();
}

void Game::Update()
{
	engine->Update();
	
	if (engine->Get<PhoenixEngine::InputSystem>()->GetKeyState(SDL_SCANCODE_ESCAPE) == PhoenixEngine::InputSystem::eKeyState::Pressed)
	{
		quit = true;
	}

	switch (state)
	{
	case Game::eState::Reset:
		Reset();
		break;
	case Game::eState::Title:
		Title();
		break;
	case Game::eState::StartGame:
		StartGame();
		break;
	case Game::eState::StartLevel:
		StartLevel();
		break;
	case Game::eState::Level:
		Level();
		break;
	case Game::eState::PlayerDead:
		PlayerDead();
		break;
	case Game::eState::GameOver:
		GameOver();
		break;
	default:
		break;
	}

	// update score
	auto scoreActor = scene->FindActor("Score");
	if (scoreActor) scoreActor->GetComponent<PhoenixEngine::TextComponent>()->SetText("SCORE: " + std::to_string(score));

	if (score > highScore) highScore = score;

	// update high score
	auto highScoreActor = scene->FindActor("HighScore");
	if (highScoreActor) highScoreActor->GetComponent<PhoenixEngine::TextComponent>()->SetText("HI-SCORE: " + std::to_string(highScore));

	//update health
	auto healthActor = scene->FindActor("Health");
	int health = 0;
	if (scene->FindActor("Player")) health = scene->FindActor("Player")->GetComponent<PlayerComponent>()->health;
	if (healthActor) healthActor->GetComponent<PhoenixEngine::TextComponent>()->SetText("HEALTH: " + std::to_string(health));

	scene->Update(engine->time.deltaTime);
}

void Game::Draw()
{
	
	engine->Get<PhoenixEngine::Renderer>()->BeginFrame();

	engine->Draw(engine->Get<PhoenixEngine::Renderer>());
	scene->Draw(engine->Get<PhoenixEngine::Renderer>());

	engine->Get<PhoenixEngine::Renderer>()->EndFrame();
}

void Game::Reset()
{
	scene->RemoveAllActors();
	score = 0;

	rapidjson::Document document;
	bool success = PhoenixEngine::json::Load("title.txt", document);
	assert(success);

	scene->Read(document);

	state = eState::Title;
}

void Game::Title()
{
	if (engine->Get<PhoenixEngine::InputSystem>()->GetKeyState(SDL_SCANCODE_SPACE) == PhoenixEngine::InputSystem::eKeyState::Pressed)
	{
		auto title = scene->FindActor("Title");
		assert(title);
		title->active = false;
		
		state = eState::StartGame;
	}
}

void Game::StartGame()
{
	rapidjson::Document document;

	bool success = PhoenixEngine::json::Load("scene.txt", document);
	assert(success);
	scene->Read(document);

	PhoenixEngine::Tilemap tilemap;
	tilemap.scene = scene.get();
	success = PhoenixEngine::json::Load("map.txt", document);
	assert(success);
	tilemap.Read(document);
	tilemap.Create();

	state = eState::StartLevel;
	stateTimer = 0;
}

void Game::StartLevel()
{
	stateTimer += engine->time.deltaTime;
	if (stateTimer >= 1)
	{
		auto player = PhoenixEngine::ObjectFactory::Instance().Create<PhoenixEngine::Actor>("Player");
		player->transform.position = PhoenixEngine::Vector2{ 400, 350 };
		scene->AddActor(std::move(player));

		spawnTimer = 2;
		state = eState::Level;
	}
}

void Game::Level()
{
	spawnTimer -= engine->time.deltaTime;
	if (spawnTimer <= 0)
	{
		spawnTimer = PhoenixEngine::RandomRange(5,8);
		auto enemy = PhoenixEngine::ObjectFactory::Instance().Create<PhoenixEngine::Actor>("Bat");
		enemy->transform.position = PhoenixEngine::Vector2{ PhoenixEngine::RandomRange(100,700), 150.0f };
		scene->AddActor(std::move(enemy));
	}

	auto player = scene->FindActor("Player");
	if (player->GetComponent<PlayerComponent>()->health <= 0) state = eState::PlayerDead;
}

void Game::PlayerDead()
{
	stateTimer = 0;
	state = eState::GameOver;
}

void Game::GameOver()
{
	auto title = scene->FindActor("Title");
	title->active = true;
	if(title) title->GetComponent<PhoenixEngine::TextComponent>()->SetText("Game Over");
	
	stateTimer += scene->engine->time.deltaTime;
	if (stateTimer >= 2)
	{
		state = eState::Reset;
	}
}

void Game::OnAddScore(const PhoenixEngine::Event event)
{
	score += std::get<int>(event.data);
}
