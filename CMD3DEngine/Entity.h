#pragma once

#include "Vec2D.h"

namespace CMD_3D_ENGINE
{
	class Entity
	{
	protected:
		Vec2D position;
		float viewAngle = 0.0f;
		float fov = 3.14159f / 4.0f;
		float rotationSpeed = 1.0f;
		float movementSpeed = 5.0f;
		float strafingSpeed = movementSpeed / 2.0f;
	public:
		Entity(const Vec2D& position, float viewAngle);

		// getters
		const Vec2D& getPosition() const;
		float getViewAngle() const;
		float getFov() const;
		float getRotationSpeed() const;
		float getMovementSpeed() const;
		float getStrafingSpeed() const;

		// setters
		void setPosition(const Vec2D& position);
		void setViewAngle(float viewAngle);
		void setFov(float fov);
		void setRotationSpeed(float rotationSpeed);
		void setMovementSpeed(float movementSpeed);
		void setStrafingSpeed(float strafingSpeed);
	};

}