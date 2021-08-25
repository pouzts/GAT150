#include "Game.h"
#include "GameComponent/PlayerComponent.h"

void Game::Initialize()
{
	// create engine
	engine = std::make_unique<PhoenixEngine::Engine>();
	engine->Startup();
	engine->Get<PhoenixEngine::Renderer>()->Create("GAT150", 800, 600);

	// register classes
	REGISTER_CLASS(PlayerComponent);

	// create scene
	scene = std::make_unique<PhoenixEngine::Scene>();
	scene->engine = engine.get();

	PhoenixEngine::SetFilePath("../Resources");

	rapidjson::Document document;
	bool success = PhoenixEngine::json::Load("scene.txt", document);
	assert(success);
	scene->Read(document);

	//std::unique_ptr<PhoenixEngine::Actor> actor = std::make_unique<PhoenixEngine::Actor>(PhoenixEngine::Transform{ {400, 300}, 0, 1 });
	//{
	//	auto component = PhoenixEngine::ObjectFactory::Instance().Create<PhoenixEngine::SpriteAnimationComponent>("SpriteAnimationComponent");
	//	component->texture = engine->Get<PhoenixEngine::ResourceSystem>()->Get<PhoenixEngine::Texture>("sprites/sparkle.png", engine->Get<PhoenixEngine::Renderer>());
	//	component->fps = 30;
	//	component->numFramesX = 8;
	//	component->numFramesY = 8;
	//	actor->AddComponent(std::move(component));
	//}
	///*{
	//	PhoenixEngine::PhysicsComponent* component = actor->AddComponent<PhoenixEngine::PhysicsComponent>();
	//	component->ApplyForce(PhoenixEngine::Vector2::right * 200);
	//}*/
	//scene->AddActor(std::move(actor));
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

