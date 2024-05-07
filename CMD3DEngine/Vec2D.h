#pragma once

namespace CMD_3D_ENGINE
{
	class Vec2D
	{
	private:
		float x;
		float y;
	public:
		Vec2D(float x, float y);
		Vec2D(const Vec2D& v);
		Vec2D& operator=(const Vec2D& right);
		~Vec2D();

		// getters
		float getX() const;
		float getY() const;

		// setters
		void setX(float x);
		void setY(float y);

		// vector math
		float magnitude() const;
		Vec2D operator+(const Vec2D& right) const;
		Vec2D& operator+=(const Vec2D& right);
		Vec2D operator-(const Vec2D& right) const;
		Vec2D& operator-=(const Vec2D& right);
		Vec2D operator*(float scalar) const;
		Vec2D& operator*=(float scalar);
		Vec2D operator/(float scalar) const;
		Vec2D& operator/=(float scalar);
		float dot(const Vec2D& v) const;
	};
}

