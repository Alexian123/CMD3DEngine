#include "Renderer.h"

#include <stdexcept>
#include <vector>
#include <algorithm>

using namespace CMD_3D_ENGINE;

Renderer::Renderer(IOHandler& ioh) : ioh(ioh), camera(0.0f, 0.0f)
{
}

void Renderer::renderScene(const Player& player, const Map& map, float elapsedTime)
{
    castRays(player, map);
    showMinimap(map, player.getPosition());
    updateWindowTitle(player, elapsedTime);
    ioh.updateScreenBuffer();
}

float Renderer::getRayDepth() const
{
	return rayDepth;
}

float Renderer::getBoundaryAngle() const
{
    return boundaryAngle;
}

float Renderer::getDistanceToWallIncrement() const
{
	return distanceToWallIncrement;
}

void Renderer::setRayDepth(float rayDepth)
{
	this->rayDepth = rayDepth;
}

void Renderer::setBoundaryAngle(float boundaryAngle)
{
    this->boundaryAngle = boundaryAngle;
}

void Renderer::setDistanceToWallIncrement(float distanceToWallIncrement)
{
	this->distanceToWallIncrement = distanceToWallIncrement;
}

void Renderer::castRays(const Player& player, const Map& map)
{
    // cast rays
    for (int x = 0; x < ioh.getScreenWidth(); ++x) {
        float rayAngle = (player.getViewAngle() - player.getFov() / 2.0f) 
            + ((float)x / (float)ioh.getScreenWidth()) * player.getFov();

        distanceToWall = 0.0f;
        hitWall = false;
        hitBoundary = false;

        // unit vector for camera direction
        camera.setX(sinf(rayAngle)); 
        camera.setY(cosf(rayAngle));

        // check if ray hit anything
        checkHit(player.getPosition(), map);

        renderLevel(x);
    }
}

void Renderer::checkHit(const Vec2D& playerPos, const Map& map)
{
    while (!hitWall && distanceToWall < rayDepth) {
        distanceToWall += distanceToWallIncrement;
        int rayX = (int)(playerPos.getX() + camera.getX() * distanceToWall);
        int rayY = (int)(playerPos.getY() + camera.getY() * distanceToWall);

        // test if ray out of bounds
        if (rayX < 0 || rayX >= map.getWidth() || rayY < 0 || rayY >= map.getHeight()) {
            hitWall = true;
            distanceToWall = rayDepth;
        }
        else if (map.isWall(rayX, rayY)) { // ray hit a wall
            hitWall = true;

            // check if ray hit a wall boundary
            checkBoundary(rayX, rayY, playerPos);
        }
    }
}

void Renderer::checkBoundary(int rayX, int rayY, const Vec2D& playerPos)
{
    static constexpr int CORNERS_PER_SIDE = 2;
    std::vector<Vec2D> boundaryData;
    for (int i = 0; i < CORNERS_PER_SIDE; ++i) {
        for (int j = 0; j < CORNERS_PER_SIDE; ++j) {
            Vec2D v((float)rayX + j - playerPos.getX(), (float)rayY + i - playerPos.getY());
            float magnitude = v.magnitude();
            v.setX(v.getX() * camera.getX());
            v.setY(v.getY() * camera.getY());
            v /= magnitude; 
            boundaryData.push_back(Vec2D(magnitude, v.getX() + v.getY()));
        }
    }

    // sort pairs from closest to farthest
    std::sort(boundaryData.begin(), boundaryData.end(),
        [](const Vec2D& a, const Vec2D& b) {
            return a.getX() < b.getX();
        });

    // check if the ray hit any of the 2 corners on the side of the block facing the player
    for (int i = 0; i < CORNERS_PER_SIDE; ++i) {
        if (acos(boundaryData[i].getY()) < boundaryAngle) {
            hitBoundary = true;
            break;
        }
    }
}

void Renderer::renderLevel(int x)
{
    // calculate distance to floor and ceiling
    int ceilingY = (int)((float)(ioh.getScreenHeight() / 2.0f) - ioh.getScreenHeight() / ((float)distanceToWall));
    int floorY = ioh.getScreenHeight() - ceilingY;

    short shade = ' ';
    if (hitBoundary) { // mark wall boundaries
        shade = ' ';
    } else { // shade walls based on distance from player
        if (distanceToWall <= rayDepth / 4.0f)        shade = 0x2588;
        else if (distanceToWall < rayDepth / 3.0f)    shade = 0x2593;
        else if (distanceToWall < rayDepth / 2.0f)    shade = 0x2592;
        else if (distanceToWall < rayDepth)           shade = 0x2591;
        else                                          shade = ' ';
    }
                                             
    for (int y = 0; y < ioh.getScreenHeight(); ++y) {
        if (y < ceilingY) {  // ceiling
            ioh.draw(x, y, ' ');
        }
        else if (y > ceilingY && y <= floorY) { // wall
            ioh.draw(x, y, shade);
        }
        else { // floor
            // shade floor based on distance
            short floorShade = ' ';
            float floorDistanceCoef = 1.0f - (((float)y - ioh.getScreenHeight() / 2.0f) / ((float)ioh.getScreenHeight() / 2.0f));
            if (floorDistanceCoef < 0.25f)      floorShade = '#';
            else if (floorDistanceCoef < 0.5f)  floorShade = '=';
            else if (floorDistanceCoef < 0.75f) floorShade = '.';
            else if (floorDistanceCoef < 0.9f)  floorShade = '-';
            ioh.draw(x, y, floorShade);
        }
    }
}

void Renderer::showMinimap(const Map& map, const Vec2D& playerPos)
{
	for (int x = 0; x < map.getWidth(); ++x) {
		for (int y = 0; y < map.getHeight(); ++y) {
            ioh.draw(x, y, map.getCell(x, y));
		}
	}

	// show player on minimap
    ioh.draw(playerPos.getX(), playerPos.getY(), map.getPlayerSymbol());
}

void Renderer::updateWindowTitle(const Player& player, float elapsedTime)
{
    static constexpr int MAX_LEN = 256; 
    wchar_t title[MAX_LEN];
    swprintf_s(title, MAX_LEN, L"CMD3DEngine - STATS: FPS=%3.2f, X=%3.2f, Y=%3.2f, A=%3.2f", 1.0 / elapsedTime,
        player.getPosition().getX(), player.getPosition().getY(), player.getViewAngle());
    ioh.setWindowTitle(title);
}