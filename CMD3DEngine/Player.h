#pragma once

#include "Entity.h"
#include "KeyboardHandler.h"
#include "Map.h"

namespace CMD_3D_ENGINE
{
	class Player : public Entity
	{
	private:
		const Map& map;
	public:
		Player(const Map &map, float x, float y, float viewAngle);
		void handleControls(const KeyboardHandler& khand, float elapsedTime);
	private:
		void move(const KeyboardHandler& khand, float elapsedTime);
		void rotate(const KeyboardHandler& khand, float elapsedTime);
	};
}

