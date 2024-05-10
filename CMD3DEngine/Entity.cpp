#include "Entity.h"

using namespace CMD_3D_ENGINE;

Entity::Entity(const Vec2D& position, float viewAngle) : position(position), viewAngle(viewAngle)
{
}

const Vec2D& Entity::getPosition() const
{
	return position;
}

float Entity::getViewAngle() const
{
	return viewAngle;
}

float Entity::getFov() const
{
	return fov;
}

float Entity::getRotationSpeed() const
{
	return rotationSpeed;
}

float Entity::getMovementSpeed() const
{
	return movementSpeed;
}

float Entity::getStrafingSpeed() const
{
	return strafingSpeed;
}

void Entity::setPosition(const Vec2D& position)
{
	this->position = position;
}

void Entity::setViewAngle(float viewAngle)
{
	this->viewAngle = viewAngle;
}

void Entity::setFov(float fov)
{
	this->fov = fov;
}

void Entity::setRotationSpeed(float rotationSpeed)
{
	this->rotationSpeed = rotationSpeed;
}

void Entity::setMovementSpeed(float movementSpeed)
{
	this->movementSpeed = movementSpeed;
}

void Entity::setStrafingSpeed(float strafingSpeed)
{
	this->strafingSpeed = strafingSpeed;
}