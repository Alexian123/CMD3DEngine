#include "Vec2D.h"

#include <cmath>

using namespace CMD_3D_ENGINE;

Vec2D::Vec2D(float x, float y) : x(x), y(y)
{
}

Vec2D::Vec2D(const Vec2D& v) : x(v.x), y(v.y)
{
}

Vec2D& Vec2D::operator=(const Vec2D& right)
{
	if (this != &right) {
		this->x = right.x;
		this->y = right.y;
	}
	return *this;
}

Vec2D::~Vec2D()
{
}

float Vec2D::getX() const
{
	return x;
}

float Vec2D::getY() const
{
	return y;
}

void Vec2D::setX(float x)
{
	this->x = x;
}

void Vec2D::setY(float y)
{
	this->y = y;
}


float Vec2D::magnitude() const
{
	return std::sqrt(x * x + y * y);
}

Vec2D Vec2D::operator+(const Vec2D& right) const
{
	return Vec2D(this->x + right.x, this->y + right.y);
}

Vec2D& Vec2D::operator+=(const Vec2D& right)
{
	x += right.x;
	y += right.y;
	return *this;
}

Vec2D Vec2D::operator-(const Vec2D& right) const
{
	return Vec2D(this->x - right.x, this->y - right.y);
}

Vec2D& Vec2D::operator-=(const Vec2D& right)
{
	x -= right.x;
	y -= right.y;
	return *this;
}

Vec2D Vec2D::operator*(float scalar) const
{
	return Vec2D(this->x * scalar, this->y * scalar);
}

Vec2D& Vec2D::operator*=(float scalar)
{
	x *= scalar;
	y *= scalar;
	return *this;
}

Vec2D Vec2D::operator/(float scalar) const
{
	return Vec2D(this->x / scalar, this->y / scalar);
}

Vec2D& Vec2D::operator/=(float scalar)
{
	x /= scalar;
	y /= scalar;
	return *this;
}

float Vec2D::dot(const Vec2D& v) const
{
	return this->x * v.x + this->y + v.y;
}