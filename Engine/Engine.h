#pragma once
// Systems
//#include "Audio/AudioSystem.h"

//Audio
#include "Audio/AudioSystem.h"

// Core
#include "Core/FileSystem.h"
#include "Core/Timer.h"
#include "Core/Utilities.h"

// Math
#include "Math/Vector2.h"
#include "Math/Color.h"
#include "Math/Random.h"
#include "Math/MathUtils.h"
#include "Math/Transform.h"

// Graphics
#include "Graphics/Renderer.h"
#include "Graphics/Texture.h"
#include "Graphics/ParticleSystem.h"
#include "Graphics/Font.h"

// Framework
#include "Framework/EventSystem.h"
#include "Resource/ResourceSystem.h"

// Objects
#include "Object/Scene.h"
#include "Object/Actor.h"

//Input
#include "Input/InputSystem.h"

#include <vector>
#include <memory>
#include <algorithm>

namespace PhoenixEngine
{
	class Engine
	{
	public:
		void Startup();
		void Shutdown();

		void Update();
		void Draw(Renderer* renderer);

		template <typename T>
		T* Get();

	public:
		FrameTimer time;

	private:
		std::vector<std::unique_ptr<System>> systems;
	};
	
	template<typename T>
	inline T* Engine::Get()
	{
		for (auto& system : systems)
		{
			if (dynamic_cast<T*>(system.get())) return dynamic_cast<T*>(system.get());
		}

		return nullptr;
	}
}