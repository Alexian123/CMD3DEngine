#pragma once

#include "Object.h"

namespace CMD_3D_ENGINE
{
	class Projectile : public Object 
	{
	private:
		Vec2D velocity;
		bool removed = false;

	public:
		Projectile();
		Projectile(const Vec2D& position, const Vec2D& velocity, std::wstring spritePath);
		Projectile(const Vec2D& position, const Vec2D& velocity, std::shared_ptr<const Sprite> sprite);
		Projectile(const Projectile& projectile);

		~Projectile() override;

		Projectile& operator=(const Projectile& right);

		const Vec2D& getVelocity() const;
		bool isRemoved() const;

		void setVelocity(const Vec2D& velocity);
		void setRemoved(bool removed);
	};


}

