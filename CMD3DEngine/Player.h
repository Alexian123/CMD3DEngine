#pragma once

#include "Entity.h"
#include "IOHandler.h"
#include "Map.h"
#include "Sprite.h"

namespace CMD_3D_ENGINE
{
	class Player : public Entity
	{
	private:
		Sprite projectileSprite;
		const Map& map;
	public:
		Player(const Vec2D position, const Map& map, float viewAngle, const Sprite& sprite);
		void handleControls(IOHandler& ioh, float elapsedTime);

		const Sprite& getProjectileSprite() const;
		void setProjectileSprite(const Sprite& projectileSprite);
	private:
		void move(const IOHandler& ioh, float elapsedTime);
		void rotate(const IOHandler& ioh, float elapsedTime);
		void fire(const IOHandler& ioh);
	};
}

