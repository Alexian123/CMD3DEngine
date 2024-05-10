#pragma once

#include "Entity.h"
#include "IOHandler.h"
#include "Map.h"
#include "Projectile.h"

namespace CMD_3D_ENGINE
{
	class Player : public Entity
	{
	private:
		const Map& map;
	public:
		Player(const Vec2D position, const Map& map, float viewAngle);
		void handleControls(IOHandler& ioh, float elapsedTime);
	private:
		void move(const IOHandler& ioh, float elapsedTime);
		void rotate(const IOHandler& ioh, float elapsedTime);
		//void fire(const IOHandler& ioh, float elapsedTime);
	};
}

