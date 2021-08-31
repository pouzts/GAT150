#pragma once
#include "Core/Serializable.h"
#include "Object/Object.h"
#include <vector>

namespace PhoenixEngine
{
	class Tilemap : public Object, public ISerializable
	{
	public:
		void Create() override;

		// Inherited via ISerializable
		virtual bool Write(const rapidjson::Value& value) const override;
		virtual bool Read(const rapidjson::Value& value) override;

	public:
		class Scene* scene = nullptr;

		int numTilesX{ 0 }, numTilesY{ 0 };
		Vector2 size;
		Vector2 start;
		std::vector<std::string> tileNames;
		std::vector<int> tiles;
	};
}