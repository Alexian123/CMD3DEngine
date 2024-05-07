#include "ScreenRenderer.h"

#include <stdexcept>
#include <vector>
#include <algorithm>

using namespace CMD_3D_ENGINE;

ScreenRenderer::ScreenRenderer(int screenWidth, int screenHeight)
	: screenWidth(screenWidth), screenHeight(screenHeight), camera(0.0f, 0.0f)
{
	if (screenWidth < 0 || screenHeight < 0) {
		throw std::out_of_range("Screen dimensions out of bounds");
	}
	screen = std::make_unique<wchar_t[]>(screenWidth * screenHeight);
	console = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, nullptr, CONSOLE_TEXTMODE_BUFFER, nullptr);
	SetConsoleActiveScreenBuffer(console);
	bytesWritten = 0;
}

void ScreenRenderer::renderScene(const Player& player, const Map& map, float elapsedTime)
{
    castRays(player, map);
    drawStats(player, elapsedTime);
    drawMinimap(map, player.getPosition());
    drawFrame();
}

int ScreenRenderer::getScreenWidth() const
{
	return screenWidth;
}

int ScreenRenderer::getSceenHeight() const
{
	return screenHeight;
}

float ScreenRenderer::getRayDepth() const
{
	return rayDepth;
}

float ScreenRenderer::getBoundaryAngle() const
{
    return boundaryAngle;
}

float ScreenRenderer::getDistanceToWallIncrement() const
{
	return distanceToWallIncrement;
}

void ScreenRenderer::setRayDepth(float rayDepth)
{
	this->rayDepth = rayDepth;
}

void ScreenRenderer::setBoundaryAngle(float boundaryAngle)
{
    this->boundaryAngle = boundaryAngle;
}

void ScreenRenderer::setDistanceToWallIncrement(float distanceToWallIncrement)
{
	this->distanceToWallIncrement = distanceToWallIncrement;
}

void ScreenRenderer::castRays(const Player& player, const Map& map)
{
    // cast rays
    for (int x = 0; x < screenWidth; ++x) {
        float rayAngle = (player.getViewAngle() - player.getFov() / 2.0f) 
            + ((float)x / (float)screenWidth) * player.getFov();

        distanceToWall = 0.0f;
        hitWall = false;
        hitBoundary = false;

        // unit vector for camera direction
        camera.setX(sinf(rayAngle)); 
        camera.setY(cosf(rayAngle));

        // check if ray hit anything
        checkHit(player.getPosition(), map);

        drawLevel(x);
    }
}

void ScreenRenderer::checkHit(const Vec2D& playerPos, const Map& map)
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

void ScreenRenderer::checkBoundary(int rayX, int rayY, const Vec2D& playerPos)
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
    //for (int i = 0; i < CORNERS_PER_SIDE; ++i) {
    if (acos(boundaryData[0].getY()) < boundaryAngle) {
        hitBoundary = true;
        //break;
    }
    if (acos(boundaryData[1].getY()) < boundaryAngle) {
        hitBoundary = true;
        //break;
    }
    //}
}

void ScreenRenderer::drawLevel(int x)
{
    // calculate distance to floor and ceiling
    int ceilingY = (int)((float)(screenHeight / 2.0f) - screenHeight / ((float)distanceToWall));
    int floorY = screenHeight - ceilingY;

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
                                             
    for (int y = 0; y < screenHeight; ++y) {
        if (y < ceilingY) {  // ceiling
            screen[y * screenWidth + x] = ' ';
        }
        else if (y > ceilingY && y <= floorY) { // wall
            screen[y * screenWidth + x] = shade;
        }
        else { // floor
            // shade floor based on distance
            short floorShade = ' ';
            float floorDistanceCoef = 1.0f - (((float)y - screenHeight / 2.0f) / ((float)screenHeight / 2.0f));
            if (floorDistanceCoef < 0.25f)      floorShade = '#';
            else if (floorDistanceCoef < 0.5f)  floorShade = '=';
            else if (floorDistanceCoef < 0.75f) floorShade = '.';
            else if (floorDistanceCoef < 0.9f)  floorShade = '-';
            screen[y * screenWidth + x] = floorShade;
        }
    }
}

void ScreenRenderer::drawStats(const Player& player, float elapsedTime)
{
	swprintf_s(screen.get(), 40, L"FPS:%3.2f, X:%3.2f, Y:%3.2f, A:%3.2f",
		1.0 / elapsedTime, player.getPosition().getX(), player.getPosition().getY(), player.getViewAngle());
}

void ScreenRenderer::drawMinimap(const Map& map, const Vec2D& playerPos)
{
	for (int x = 0; x < map.getWidth(); ++x) {
		for (int y = 0; y < map.getHeight(); ++y) {
			screen[(y + 1) * screenWidth + x] = map.getCell(x, y);
		}
	}

	// show player on minimap
	screen[((int)playerPos.getY() + 1) * screenWidth + (int)playerPos.getX()] = map.getPlayerSymbol();
}

void ScreenRenderer::drawFrame()
{
	screen[screenWidth * screenHeight - 1] = '\0';	// end of screen buffer
	WriteConsoleOutputCharacter(console, screen.get(), screenWidth * screenHeight, {0, 0}, &bytesWritten);
}