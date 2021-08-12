#include "Game.h"
#include "Object/Actor.h"
#include "Resource/ResourceSystem.h"

void Game::Initialize()
{
	engine = std::make_unique<PhoenixEngine::Engine>();
	engine->Startup();
	engine->Get<PhoenixEngine::Renderer>()->Create("GAT150", 800, 600);

	scene = std::make_unique<PhoenixEngine::Scene>();
	scene->engine = engine.get();

	PhoenixEngine::SetFilePath("../Resources");

	std::shared_ptr<PhoenixEngine::Texture> texture = engine->Get<PhoenixEngine::ResourceSystem>()->Get<PhoenixEngine::Texture>("sf2.png", engine->Get<PhoenixEngine::Renderer>());

	engine->Get<PhoenixEngine::AudioSystem>()->AddAudio("explosion", "audio/explosion.wav");
	engine->Get<PhoenixEngine::AudioSystem>()->AddAudio("music", "audio/music.mp3");
	musicChannel = engine->Get<PhoenixEngine::AudioSystem>()->PlayAudio("music", 1, 1, true);

	for (int i = 0; i < 200; i++)
	{
		PhoenixEngine::Transform transform{ PhoenixEngine::Vector2{PhoenixEngine::RandomRange(0.0f, 800.0f), PhoenixEngine::RandomRange(0.0f, 600.0f)}, PhoenixEngine::RandomRange(0.0f, 360.0f), 1.0f };
		std::unique_ptr<PhoenixEngine::Actor> actor = std::make_unique<PhoenixEngine::Actor>(transform, texture);

		scene->AddActor(std::move(actor));
	}

	// get font from resource system
	int size = 16;
	std::shared_ptr<PhoenixEngine::Font> font = engine->Get<PhoenixEngine::ResourceSystem>()->Get<PhoenixEngine::Font>("fonts/November.ttf", &size);

	// create font texture
	textTexture = std::make_shared<PhoenixEngine::Texture>(engine->Get<PhoenixEngine::Renderer>());
	// set font texture with font surface
	textTexture->Create(font->CreateSurface("HELLO WORLD", PhoenixEngine::Color{ PhoenixEngine::Random(), PhoenixEngine::Random(), PhoenixEngine::Random() }));
	// add font texture to resource system
	engine->Get<PhoenixEngine::ResourceSystem>()->Add("textTexture", textTexture);
	
	particleTexture = engine->Get<PhoenixEngine::ResourceSystem>()->Get<PhoenixEngine::Texture>("particle01.png", engine->Get<PhoenixEngine::Renderer>());

	engine->Get<PhoenixEngine::AudioSystem>()->AddAudio("playerFire", "player_fire.wav");
	engine->Get<PhoenixEngine::AudioSystem>()->AddAudio("enemyFire", "enemy_fire.wav");

	engine->Get<PhoenixEngine::AudioSystem>()->AddAudio("explosion", "explosion.wav");
	engine->Get<PhoenixEngine::AudioSystem>()->AddAudio("hurt", "hurt.wav");

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
	float dt = engine->time.deltaTime;

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
		//health = scene->GetActor<Player>()->GetHealth();
		/*if (scene->GetActors<Astroid>().size() == 0 && scene->GetActors<Enemy>().size() == 0)
		{
			state = eState::EndLevel;
			stateTimer = 0;
			level++;
		}*/
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

	engine->Update();
	scene->Update(dt);

	if (engine->Get<PhoenixEngine::InputSystem>()->GetKeyState(SDL_SCANCODE_ESCAPE) == PhoenixEngine::InputSystem::eKeyState::Pressed)
	{
		quit = true;
	}

	if (engine->Get<PhoenixEngine::InputSystem>()->GetButtonState((int)PhoenixEngine::InputSystem::eMouseButton::Left) == PhoenixEngine::InputSystem::eKeyState::Pressed)
	{
		PhoenixEngine::Vector2 position = engine->Get<PhoenixEngine::InputSystem>()->GetMousePosition();
		// Create Particle System
		engine->Get<PhoenixEngine::ParticleSystem>()->Create(position, 50, 5.0f, particleTexture, 300.0f);
		engine->Get<PhoenixEngine::AudioSystem>()->PlayAudio("explosion", 1, PhoenixEngine::RandomRange(0.2f, 2.0f));
		musicChannel.SetPitch(PhoenixEngine::RandomRange(0.5f, 2.0f));
		//std::cout << position.x << " " << position.y << std::endl;
	}

	scene->Update(engine->time.deltaTime);

	//std::cout << engine->time.time << std::endl;
	//if (engine->time.time >= quitTime) quit = true;
	engine->time.timeScale = 2;
}

void Game::Draw()
{
	switch (state)
	{
	case Game::eState::Title:
		//DrawTitle(renderer);
		break;
	case Game::eState::StartGame:
		break;
	case Game::eState::StartLevel:
		break;
	case Game::eState::Game:
		break;
	case Game::eState::EndLevel:
	{
		std::string levelname = "Level " + std::to_string(level);
		/*graphics.SetColor(PhoenixEngine::Color::white);
		graphics.DrawString(350, 400, levelname.c_str());*/
	}
	break;
	case Game::eState::GameOver:
		/*graphics.SetColor(PhoenixEngine::Color::white);
		graphics.DrawString(350, 400, "Game Over");*/
		break;
	default:
		break;
	}

	/*graphics.SetColor(PhoenixEngine::Color::blue);
	graphics.DrawString(30, 20, "SCORE: ");
	graphics.SetColor(PhoenixEngine::Color::white);
	graphics.DrawString(80, 20, std::to_string(score).c_str());
	graphics.SetColor(PhoenixEngine::Color::red);
	graphics.DrawString(720, 20, "HEALTH: ");
	graphics.SetColor(PhoenixEngine::Color::white);
	graphics.DrawString(770, 20, std::to_string(health).c_str());*/

	// draw
	engine->Get<PhoenixEngine::Renderer>()->BeginFrame();

	scene->Draw(engine->Get<PhoenixEngine::Renderer>());
	engine->Draw(engine->Get<PhoenixEngine::Renderer>());

	PhoenixEngine::Transform t;
	t.position = { 30, 30 };
	engine->Get<PhoenixEngine::Renderer>()->Draw(textTexture, t);

	engine->Get<PhoenixEngine::Renderer>()->EndFrame();
}

void Game::DrawTitle(PhoenixEngine::Renderer* renderer)
{
	/*graphics.SetColor(PhoenixEngine::Color(PhoenixEngine::Random(), PhoenixEngine::Random(), PhoenixEngine::Random()));
	engine->Get<PhoenixEngine::ResourceSystem>()->Get<PhoenixEngine::Shape>("logov.txt")->Draw(graphics, PhoenixEngine::Vector2{ 150, 300 }, 0, 3);
	engine->Get<PhoenixEngine::ResourceSystem>()->Get<PhoenixEngine::Shape>("logoe.txt")->Draw(graphics, PhoenixEngine::Vector2{ 250, 300 }, 0, 3);
	engine->Get<PhoenixEngine::ResourceSystem>()->Get<PhoenixEngine::Shape>("logor.txt")->Draw(graphics, PhoenixEngine::Vector2{ 350, 300 }, 0, 3);
	engine->Get<PhoenixEngine::ResourceSystem>()->Get<PhoenixEngine::Shape>("logot.txt")->Draw(graphics, PhoenixEngine::Vector2{ 450, 300 }, 0, 3);
	engine->Get<PhoenixEngine::ResourceSystem>()->Get<PhoenixEngine::Shape>("logoe.txt")->Draw(graphics, PhoenixEngine::Vector2{ 550, 300 }, 0, 3);
	engine->Get<PhoenixEngine::ResourceSystem>()->Get<PhoenixEngine::Shape>("logox.txt")->Draw(graphics, PhoenixEngine::Vector2{ 650, 300 }, 0, 3);

	graphics.SetColor(PhoenixEngine::Color::white);
	graphics.DrawString(325, 400, "Press SPACE to continue");*/
}

void Game::UpdateTitle(float dt)
{
	/*if (engine->Get<PhoenixEngine::InputSystem>()->GetKeyState(SDL_SCANCODE_SPACE) == PhoenixEngine::InputSystem::eKeyState::Pressed) {
		state = eState::StartGame;
	}*/
}

void Game::UpdateLevelStart(float dt)
{
	//scene->AddActor(std::make_unique<Player>(PhoenixEngine::Transform{ PhoenixEngine::Vector2{400, 300}, 0, 3 }, engine->Get<PhoenixEngine::ResourceSystem>()->Get<PhoenixEngine::Shape>("playershape.txt"), 300.0f));

	//for (size_t i = 0; i < level; i++)
	//{
	//	bool hasSpawned = false;
	//	while (!hasSpawned) {
	//		float spawnX = PhoenixEngine::RandomRange(0.0f, 800.0f);
	//		float spawnY = PhoenixEngine::RandomRange(0.0f, 600.0f);
	//		if ((spawnX < 300.0f || spawnX > 500.0f) && (spawnY < 200.0f || spawnY > 400.0f)) 
	//		{
	//			//scene->AddActor(std::make_unique<Enemy>(PhoenixEngine::Transform{ PhoenixEngine::Vector2{spawnX, spawnY}, PhoenixEngine::RandomRange(0.0f, PhoenixEngine::TwoPi), 3 }, engine->Get<PhoenixEngine::ResourceSystem>()->Get<PhoenixEngine::Shape>("enemyshape.txt"), 150.0f));
	//			hasSpawned = true;
	//		}
	//	}
	//}

	//for (size_t i = 0; i < 2 + level; i++)
	//{
	//	bool hasSpawned = false;
	//	while (!hasSpawned) {
	//		float spawnX = PhoenixEngine::RandomRange(0.0f, 800.0f);
	//		float spawnY = PhoenixEngine::RandomRange(0.0f, 600.0f);
	//		if ((spawnX < 300.0f || spawnX > 500.0f) && (spawnY < 200.0f || spawnY > 400.0f))
	//		{
	//			//scene->AddActor(std::make_unique<Astroid>(PhoenixEngine::Transform{ PhoenixEngine::Vector2{spawnX, spawnY}, PhoenixEngine::RandomRange(0.0f, PhoenixEngine::TwoPi), 3 }, engine->Get<PhoenixEngine::ResourceSystem>()->Get<PhoenixEngine::Shape>("astroidshape.txt"), 150.0f));
	//			hasSpawned = true;
	//		}
	//	}
	//}

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
