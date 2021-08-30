#include "Game.h"
#include "GameComponent/PlayerComponent.h"
#include "GameComponent/EnemyComponent.h"
#include "GameComponent/PickupComponent.h"

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

	// create scene
	scene = std::make_unique<PhoenixEngine::Scene>();
	scene->engine = engine.get();

	PhoenixEngine::SetFilePath("../Resources");

	rapidjson::Document document;
	bool success = PhoenixEngine::json::Load("scene.txt", document);
	assert(success);
	scene->Read(document);

	for (size_t i = 0; i < 10; i++) 
	{
		auto actor = PhoenixEngine::ObjectFactory::Instance().Create<PhoenixEngine::Actor>("Coin");
		actor->transform.position = PhoenixEngine::Vector2{PhoenixEngine::RandomRange(0,800), PhoenixEngine::RandomRange(250,300)};
		scene->AddActor(std::move(actor));
	}
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

	scene->Update(engine->time.deltaTime);
}

void Game::Draw()
{
	
	engine->Get<PhoenixEngine::Renderer>()->BeginFrame();

	engine->Draw(engine->Get<PhoenixEngine::Renderer>());
	scene->Draw(engine->Get<PhoenixEngine::Renderer>());

	engine->Get<PhoenixEngine::Renderer>()->EndFrame();
}

