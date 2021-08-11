#include "Engine.h"
#include <SDL.h>
#include <SDL_Image.h>
#include <iostream>
#include <windows.h>

using timer_t = DWORD;

int main(int, char**)
{
	PhoenixEngine::Engine engine;
	engine.Startup();
	engine.Get<PhoenixEngine::Renderer>()->Create("GAT150", 800, 600);

	PhoenixEngine::Scene scene;

	scene.engine = &engine;

	PhoenixEngine::SetFilePath("../Resources");

	std::shared_ptr<PhoenixEngine::Texture> texture = engine.Get<PhoenixEngine::ResourceSystem>()->Get<PhoenixEngine::Texture>("sf2.png", engine.Get<PhoenixEngine::Renderer>());
	
	engine.Get<PhoenixEngine::AudioSystem>()->AddAudio("explosion", "audio/explosion.wav");
	engine.Get<PhoenixEngine::AudioSystem>()->AddAudio("music", "audio/music.mp3");
	PhoenixEngine::AudioChannel channel = engine.Get<PhoenixEngine::AudioSystem>()->PlayAudio("music", 1, 1, true);

	for (int i = 0; i < 200; i++)
	{
		PhoenixEngine::Transform transform{ PhoenixEngine::Vector2{PhoenixEngine::RandomRange(0.0f, 800.0f), PhoenixEngine::RandomRange(0.0f, 600.0f)}, PhoenixEngine::RandomRange(0.0f, 360.0f), 1.0f };
		std::unique_ptr<PhoenixEngine::Actor> actor = std::make_unique<PhoenixEngine::Actor>(transform, texture);

		scene.AddActor(std::move(actor));
	}

	// get font from resource system
	int size = 16;
	std::shared_ptr<PhoenixEngine::Font> font = engine.Get<PhoenixEngine::ResourceSystem>()->Get<PhoenixEngine::Font>("fonts/November.ttf", &size);

	// create font texture
	std::shared_ptr<PhoenixEngine::Texture> textTexture = std::make_shared<PhoenixEngine::Texture>(engine.Get<PhoenixEngine::Renderer>());
	// set font texture with font surface
	textTexture->Create(font->CreateSurface("hello world", PhoenixEngine::Color{ 1, 1, 1 }));
	// add font texture to resource system
	engine.Get<PhoenixEngine::ResourceSystem>()->Add("textTexture", textTexture);


	bool quit = false;
	SDL_Event event;
	float quitTime = engine.time.time + 3.0f;

	while (!quit)
	{
		SDL_PollEvent(&event);
		switch (event.type)
		{
			case SDL_QUIT:
				quit = true;
				break;
		}

		//update
		engine.Update();
		if (engine.Get<PhoenixEngine::InputSystem>()->GetKeyState(SDL_SCANCODE_ESCAPE) == PhoenixEngine::InputSystem::eKeyState::Pressed)
		{
			quit = true;
		}
		
		if (engine.Get<PhoenixEngine::InputSystem>()->GetButtonState((int)PhoenixEngine::InputSystem::eMouseButton::Left) == PhoenixEngine::InputSystem::eKeyState::Pressed)
		{
			PhoenixEngine::Vector2 position = engine.Get<PhoenixEngine::InputSystem>()->GetMousePosition();
			// Create Particle System
			std::shared_ptr<PhoenixEngine::Texture> particleTexture = engine.Get<PhoenixEngine::ResourceSystem>()->Get<PhoenixEngine::Texture>("particle01.png", engine.Get<PhoenixEngine::Renderer>());
			engine.Get<PhoenixEngine::ParticleSystem>()->Create(position, 50, 5.0f, particleTexture, 300.0f);
			engine.Get<PhoenixEngine::AudioSystem>()->PlayAudio("explosion", 1, PhoenixEngine::RandomRange(0.2f, 2.0f));
			channel.SetPitch(PhoenixEngine::RandomRange(0.5f, 2.0f));
			//std::cout << position.x << " " << position.y << std::endl;
		}

		scene.Update(engine.time.deltaTime);

		//std::cout << engine.time.time << std::endl;
		//if (engine.time.time >= quitTime) quit = true;
		engine.time.timeScale = 2;

		// draw
		engine.Get<PhoenixEngine::Renderer>()->BeginFrame();

		scene.Draw(engine.Get<PhoenixEngine::Renderer>());
		engine.Draw(engine.Get<PhoenixEngine::Renderer>());

		PhoenixEngine::Transform t;
		t.position = { 30, 30 };
		engine.Get<PhoenixEngine::Renderer>()->Draw(textTexture, t);

		engine.Get<PhoenixEngine::Renderer>()->EndFrame();

	}

	SDL_Quit();

	return 0;
}
