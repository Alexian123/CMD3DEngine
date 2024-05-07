#include "Player.h"

using namespace CMD_3D_ENGINE;

Player::Player(const Map& map, float x, float y, float viewAngle) : Entity(x, y, viewAngle), map(map)
{
}

void Player::handleControls(const KeyboardHandler& khand, float elapsedTime)
{
    move(khand, elapsedTime);
    rotate(khand, elapsedTime);
}

void Player::move(const KeyboardHandler& khand, float elapsedTime)
{
    if (khand.isKeyPressed(W)) {    // forward
        Vec2D offset(sinf(viewAngle) * movementSpeed * elapsedTime, cosf(viewAngle) * movementSpeed * elapsedTime);
        position += offset;

        // check wall collision
        if (map.isWall((int)position.getX(), (int)position.getY())) {
            position -= offset;
        }
    }
    if (khand.isKeyPressed(A)) {    // left
        Vec2D offset((-1.0f) * cosf(viewAngle) * strafingSpeed * elapsedTime, sinf(viewAngle) * strafingSpeed * elapsedTime);
        position += offset;

        // check wall collision
        if (map.isWall((int)position.getX(), (int)position.getY())) {
            position -= offset;
        }
    }
    if (khand.isKeyPressed(S)) { // backwards
        Vec2D offset(sinf(viewAngle) * movementSpeed * elapsedTime, cosf(viewAngle) * movementSpeed * elapsedTime);
        position -= offset;

        // check wall collision
        if (map.isWall((int)position.getX(), (int)position.getY())) {
            position += offset;
        }
    }
    if (khand.isKeyPressed(D)) {    // right
        Vec2D offset(cosf(viewAngle) * strafingSpeed * elapsedTime, (-1.0f) * sinf(viewAngle) * strafingSpeed * elapsedTime);
        position += offset;

        // check wall collision
        if (map.isWall((int)position.getX(), (int)position.getY())) {
            position -= offset;
        }
    }
}

void Player::rotate(const KeyboardHandler& khand, float elapsedTime)
{
    if (khand.isKeyPressed(Q)) {
        viewAngle -= rotationSpeed * elapsedTime;
    }
    if (khand.isKeyPressed(E)) {
        viewAngle += rotationSpeed * elapsedTime;
    }
}
