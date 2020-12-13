#pragma once

#include "Component.h"
#include "../Renderable2D.h"
#include "../AssetManager.h"

namespace Plutus
{
	class TileComponent : public Component, public Renderable2D
	{
	public:
		TileComponent(const glm::vec2& pos, const glm::vec2& size, const std::string texid, int tid )
		{
			auto ts = AssetManager::getTileSet(texid);
			mPosition = pos;
			mSize = size;
			mUVCoord = ts->getUV(tid);
;
			mTid = ts->texture.id;
		}

		void Initialize() override {}

		void Update(float deltaTime) override {}

		void Serialize(Serializer& serializer) const override {}
	};
}
