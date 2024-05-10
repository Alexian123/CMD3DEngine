#include "Player.h"
#include "Projectile.h"

#include <memory>

using namespace CMD_3D_ENGINE;

Player::Player(const Vec2D position, const Map& map, float viewAngle, const Sprite& projectileSprite)
    : Entity(position, viewAngle), map(map), projectileSprite(projectileSprite)
{
}

void Player::handleControls(IOHandler& ioh, float elapsedTime)
{
    ioh.readInput();
    move(ioh, elapsedTime);
    rotate(ioh, elapsedTime);
    fire(ioh);
}

const Sprite& Player::getProjectileSprite() const
{
    return projectileSprite;
}

void Player::setProjectileSprite(const Sprite& projectileSprite)
{
    this->projectileSprite = projectileSprite;
}

void Player::move(const IOHandler& ioh, float elapsedTime)
{
    if (ioh.getKeyState(W_KEY).held) {    // forward
        Vec2D offset(sinf(viewAngle) * movementSpeed * elapsedTime, cosf(viewAngle) * movementSpeed * elapsedTime);
        position += offset;

        // check wall collision
        if (map.isWall((int)position.getX(), (int)position.getY())) {
            position -= offset;
        }
    }
    if (ioh.getKeyState(A_KEY).held) {    // left
        Vec2D offset((-1.0f) * cosf(viewAngle) * strafingSpeed * elapsedTime, sinf(viewAngle) * strafingSpeed * elapsedTime);
        position += offset;

        // check wall collision
        if (map.isWall((int)position.getX(), (int)position.getY())) {
            position -= offset;
        }
    }
    if (ioh.getKeyState(S_KEY).held) { // backwards
        Vec2D offset(sinf(viewAngle) * movementSpeed * elapsedTime, cosf(viewAngle) * movementSpeed * elapsedTime);
        position -= offset;

        // check wall collision
        if (map.isWall((int)position.getX(), (int)position.getY())) {
            position += offset;
        }
    }
    if (ioh.getKeyState(D_KEY).held) {    // right
        Vec2D offset(cosf(viewAngle) * strafingSpeed * elapsedTime, (-1.0f) * sinf(viewAngle) * strafingSpeed * elapsedTime);
        position += offset;

        // check wall collision
        if (map.isWall((int)position.getX(), (int)position.getY())) {
            position -= offset;
        }
    }
}

void Player::rotate(const IOHandler& ih, float elapsedTime)
{
    if (ih.getKeyState(Q_KEY).held) {
        viewAngle -= rotationSpeed * elapsedTime;
    }
    if (ih.getKeyState(E_KEY).held) {
        viewAngle += rotationSpeed * elapsedTime;
    }
}

void Player::fire(const IOHandler& ioh)
{
    if (ioh.getKeyState(SPACE_KEY).released || ioh.getMouseState(MOUSE_LEFT_BTN).pressed) {
        float noise = (((float)rand() / (float)RAND_MAX) - 0.5f) * 0.1f;
        Vec2D velocity(sinf(viewAngle + noise) * 8.0f, cosf(viewAngle + noise) * 8.0f);
        std::shared_ptr<const Sprite> projectileSpritePtr(&projectileSprite);
        
        // spawn a projectile; it will automatically get added to the list of objects & freed upon death
        new Projectile(position, velocity, projectileSpritePtr);
    }
}