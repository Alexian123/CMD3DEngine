#pragma once

#include <Windows.h>
#include <memory>

#include "IOHandler.h"
#include "Player.h"
#include "Map.h"
#include "FrameTimer.h"

namespace CMD_3D_ENGINE
{
	class Renderer
	{
	private:
		IOHandler& ioh;
		Vec2D camera;
		float rayDepth = 16.0f;
		float boundaryAngle = 0.01f;
		float distanceToWall = 0.0f;
		float distanceToWallIncrement = 0.1f;
		bool hitWall = false;
		bool hitBoundary = false;
	public:
		Renderer(IOHandler& ioh);

		void renderScene(const Player& player, const Map& map, float elapsedTime);

		// getters
		float getRayDepth() const;
		float getBoundaryAngle() const;
		float getDistanceToWallIncrement() const;

		// setters
		void setRayDepth(float rayDepth);
		void setBoundaryAngle(float boundaryAngle);
		void setDistanceToWallIncrement(float distanceToWallIncrement);
	private:
		void castRays(const Player& player, const Map& map);
		void checkHit(const Vec2D& playerPos, const Map& map);
		void checkBoundary(int rayX, int rayY, const Vec2D& playerPos);
		void renderLevel(int x);
		void showMinimap(const Map& map, const Vec2D& playerPos);
		void updateWindowTitle(const Player& player, float elapsedTime);
	};
}


