#pragma once

#include "Entity.h"
#include "IOHandler.h"
#include "Map.h"

namespace CMD_3D_ENGINE
{
	class Player : public Entity
	{
	private:
		const Map& map;
	public:
		Player(const Map &map, float x, float y, float viewAngle);
		void handleControls(IOHandler& ioh, float elapsedTime);
	private:
		void move(const IOHandler& ioh, float elapsedTime);
		void rotate(const IOHandler& ioh, float elapsedTime);
	};
}

