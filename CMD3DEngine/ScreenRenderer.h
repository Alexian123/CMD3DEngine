#pragma once

#include <Windows.h>
#include <memory>

#include "Player.h"
#include "Map.h"
#include "FrameTimer.h"

namespace CMD_3D_ENGINE
{
	class ScreenRenderer
	{
	private:
		const int screenWidth;
		const int screenHeight;

		std::unique_ptr<wchar_t[]> screen;	// screen buffer
		HANDLE console;
		DWORD bytesWritten;
		
		Vec2D camera;
		float rayDepth = 16.0f;
		float boundaryAngle = 0.01f;
		float distanceToWall = 0.0f;
		float distanceToWallIncrement = 0.1f;
		bool hitWall = false;
		bool hitBoundary = false;
	public:
		ScreenRenderer(int screenWidth, int screenHeight);

		void renderScene(const Player& player, const Map& map, float elapsedTime);

		// getters
		int getScreenWidth() const;
		int getSceenHeight() const;
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
		void drawLevel(int x);
		void drawStats(const Player& player, float elapsedTime);
		void drawMinimap(const Map& map, const Vec2D& playerPos);
		void drawFrame();
	};
}


