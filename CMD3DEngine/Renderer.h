#pragma once

#include <Windows.h>
#include <memory>

#include "IOHandler.h"
#include "Sprite.h"
#include "Player.h"
#include "Object.h"
#include "Map.h"
#include "FrameTimer.h"

namespace CMD_3D_ENGINE
{
	class Renderer
	{
	private:
		IOHandler& ioh;
		Sprite wall;
		Vec2D camera;
		float rayDepth = 16.0f;
		float distanceToWall = 0.0f;
		float distanceToWallIncrement = 0.01f;
		float minObjectDrawDistance = 0.5f; // objects will not be drawn if they are too close to the player
		bool rayHitWall = false;
		std::unique_ptr<float[]> depthBuffer = nullptr;

	public:
		Renderer(IOHandler& ioh, std::wstring wallSpritePath);

		void renderScene(const Player& player, const Map& map, float elapsedTime);

		// getters
		float getRayDepth() const;
		float getDistanceToWallIncrement() const;
		float getMinObjectDrawDistance() const;

		// setters
		void setRayDepth(float rayDepth);
		void setDistanceToWallIncrement(float distanceToWallIncrement);
		void setMinObjectDrawDistance(float minObjectDrawDistance);

	private:
		void renderLevel(const Player& player, const Map& map);
		void renderObjects(const Player& player, const Map& map, float elapsedTime);
		void showMinimap(const Map& map, const Vec2D& playerPos);
		void updateWindowTitle(const Player& player, float elapsedTime);
	};
}


