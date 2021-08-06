#pragma once
#include "Framework/System.h"
#include <SDL.h>
#include <vector>
#include <algorithm>

namespace PhoenixEngine
{
	class InputSystem : public System
	{
	public:
		enum class eKeyState
		{
			Idle,
			Pressed,
			Held,
			Released
		};

	public:
		void Startup() override;
		void Shutdown() override;
		void Update(float dt) override;

		eKeyState GetKeyState(int id);
		bool IsKeyDown(int id);
		bool IsPreviousKeyDown(int id);
	
	private:
		std::vector<Uint8> keyboardState;
		std::vector<Uint8> prevKeyboardState;
		int numKeys;
	};
}