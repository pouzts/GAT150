#include "Game.h"

void Game::Initialize()
{
	engine = std::make_unique<PhoenixEngine::Engine>();
	engine->Startup();
	engine->Get<PhoenixEngine::Renderer>()->Create("GAT150", 800, 600);

	scene = std::make_unique<PhoenixEngine::Scene>();
	scene->engine = engine.get();

	PhoenixEngine::SetFilePath("../Resources");

	std::unique_ptr<PhoenixEngine::Actor> actor = std::make_unique<PhoenixEngine::Actor>(PhoenixEngine::Transform{ {400, 300} });
	{
		std::unique_ptr<PhoenixEngine::SpriteComponent> component = std::make_unique<PhoenixEngine::SpriteComponent>();
		component->texture = engine->Get<PhoenixEngine::ResourceSystem>()->Get<PhoenixEngine::Texture>("sf2.png", engine->Get<PhoenixEngine::Renderer>());
		actor->AddComponent(std::move(component));
	}
	{
		std::unique_ptr<PhoenixEngine::PhysicsComponent> component = std::make_unique<PhoenixEngine::PhysicsComponent>();
		component->ApplyForce(PhoenixEngine::Vector2::right * 200);
		actor->AddComponent(std::move(component));
	}
	scene->AddActor(std::move(actor));
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

