#pragma once

#include "Object.h"

namespace CMD_3D_ENGINE
{
	class Projectile : public Object 
	{
	private:
		Vec2D velocity;

	public:
		Projectile();
		Projectile(const Vec2D& position, const Vec2D& velocity, std::wstring spritePath);
		Projectile(const Vec2D& position, const Vec2D& velocity, std::shared_ptr<const Sprite> sprite);
		Projectile(const Projectile& projectile);

		~Projectile() override;

		Projectile& operator=(const Projectile& right);

		void updatePhysics(float elapsedTime) override;

		const Vec2D& getVelocity() const;
		void setVelocity(const Vec2D& velocity);
	};


}

