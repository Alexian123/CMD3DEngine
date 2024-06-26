#include "Projectile.h"

using namespace CMD_3D_ENGINE;


Projectile::Projectile() : Object(), velocity(0.0f, 0.0f)
{
}

Projectile::Projectile(const Vec2D& position, const Vec2D& velocity, std::wstring spritePath)
    : Object(position, spritePath), velocity(velocity)
{
}

Projectile::Projectile(const Vec2D& position, const Vec2D& velocity, std::shared_ptr<const Sprite> sprite)
    : Object(position, sprite), velocity(velocity)
{
}

Projectile::Projectile(const Projectile& projectile) 
    : Object(projectile), velocity(projectile.velocity)
{
}

Projectile::~Projectile()
{
}

Projectile& Projectile::operator=(const Projectile& right)
{
    if (this != &right) {
        Object::operator=(right);
        this->velocity = right.velocity;
    }
    return *this;
}

void Projectile::updatePhysics(float elapsedTime)
{
    position += (velocity * elapsedTime);
}

const Vec2D& Projectile::getVelocity() const
{
    return velocity;
}

void Projectile::setVelocity(const Vec2D& velocity)
{
    this->velocity = velocity;
}