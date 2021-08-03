#pragma once

#include "Framework/System.h"
//#include <fmod.hpp>
#include <string>
#include <map>

namespace PhoenixEngine
{
	class AudioSystem : public System
	{
	public:
		void Startup();
		void Shutdown();

		void Update(float dt);

		void AddAudio(const std::string& name, const std::string& filename);
		void PlayAudio(const std::string& name);

	private:
		//FMOD::System* fmodSystem;
		//std::map<std::string, FMOD::Sound*> sounds;
	};
}