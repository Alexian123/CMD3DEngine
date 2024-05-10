#include "Renderer.h"

#include <stdexcept>
#include <vector>
#include <algorithm>

#define PI_F 3.14159f

using namespace CMD_3D_ENGINE;

Renderer::Renderer(IOHandler& ioh, std::wstring wallSpritePath) : ioh(ioh), wall(wallSpritePath), camera(0.0f, 0.0f)
{
    // allocate depth buffer
    depthBuffer = std::make_unique<float[]>(ioh.getScreenWidth());
}

void Renderer::renderScene(const Player& player, const Map& map, float elapsedTime)
{
    renderLevel(player, map);
    renderObjects(player);
    showMinimap(map, player.getPosition());
    updateWindowTitle(player, elapsedTime);
    ioh.updateScreenBuffer();
}

float Renderer::getRayDepth() const
{
	return rayDepth;
}

float Renderer::getDistanceToWallIncrement() const
{
	return distanceToWallIncrement;
}

float Renderer::getMinObjectDrawDistance() const
{
    return minObjectDrawDistance;
}

void Renderer::setRayDepth(float rayDepth)
{
	this->rayDepth = rayDepth;
}

void Renderer::setDistanceToWallIncrement(float distanceToWallIncrement)
{
	this->distanceToWallIncrement = distanceToWallIncrement;
}

void Renderer::setMinObjectDrawDistance(float minObjectDrawDistance)
{
    this->minObjectDrawDistance = minObjectDrawDistance;
}

void Renderer::renderLevel(const Player& player, const Map& map)
{
    const Vec2D& playerPos = player.getPosition();

    // cast rays
    for (int x = 0; x < ioh.getScreenWidth(); ++x) {
        float rayAngle = (player.getViewAngle() - player.getFov() / 2.0f) + ((float)x / (float)ioh.getScreenWidth()) * player.getFov();

        distanceToWall = 0.0f;
        rayHitWall = false;
        
        // unit vector for camera direction
        camera.setX(sinf(rayAngle)); 
        camera.setY(cosf(rayAngle));

        float sampleX = 0.0f;

        // check if ray hit anything
        while (!rayHitWall && distanceToWall < rayDepth) {
            distanceToWall += distanceToWallIncrement;
            int rayX = (int)(playerPos.getX() + camera.getX() * distanceToWall);
            int rayY = (int)(playerPos.getY() + camera.getY() * distanceToWall);

            // test if ray out of bounds
            if (rayX < 0 || rayX >= map.getWidth() || rayY < 0 || rayY >= map.getHeight()) {
                rayHitWall = true;
                distanceToWall = rayDepth;
            }
            else if (map.isWall(rayX, rayY)) { // ray hit a wall
                rayHitWall = true;

                // determine where the ray hit the wall
                Vec2D blockMid((float)rayX + 0.5f, (float)rayY + 0.5f);
                Vec2D testPoint(playerPos.getX() + camera.getX() * distanceToWall, playerPos.getY() + camera.getY() * distanceToWall);
                float testAngle = atan2f(testPoint.getY() - blockMid.getY(), testPoint.getX() - blockMid.getX());

                if (testAngle >= -PI_F * 0.25f && testAngle < PI_F * 0.25f) {
                    sampleX = testPoint.getY() - (float)rayY;
                }
                if (testAngle >= PI_F * 0.25f && testAngle < PI_F * 0.75f) {
                    sampleX = testPoint.getX() - (float)rayX;
                }
                if (testAngle < -PI_F * 0.25f && testAngle >= -PI_F * 0.75f) {
                    sampleX = testPoint.getX() - (float)rayX;
                }
                if (testAngle >= PI_F * 0.75f && testAngle < -PI_F * 0.75f) {
                    sampleX = testPoint.getY() - (float)rayY;
                }
            }
        }

        // calculate distance to floor and ceiling
        int ceilingY = (int)((float)(ioh.getScreenHeight() / 2.0f) - ioh.getScreenHeight() / ((float)distanceToWall));
        int floorY = ioh.getScreenHeight() - ceilingY;

        // update depth buffer
        depthBuffer[x] = distanceToWall;

        // iterate row by row
        for (int y = 0; y < ioh.getScreenHeight(); ++y) {
            if (y <= ceilingY) {  // ceiling
                ioh.drawGlyph(x, y, PIXEL_SOLID, FG_DARK_BLUE);
            }
            else if (y > ceilingY && y <= floorY) { // wall
                if (distanceToWall < rayDepth) {
                    float sampleY = ((float)y - (float)ceilingY) / ((float)floorY - (float)ceilingY);
                    ioh.drawGlyph(x, y, wall.sampleGlyph(sampleX, sampleY), wall.sampleColor(sampleX, sampleY));
                }
                else {
                    ioh.drawGlyph(x, y, PIXEL_SOLID, FG_BLACK);
                }
            }
            else { // floor
                ioh.drawGlyph(x, y, PIXEL_SOLID, FG_DARK_GREEN);
            }
        }
    }
}

void Renderer::renderObjects(const Player& player)
{
    const std::list<std::shared_ptr<Object>>& objects = Object::getObjectList();
    const Vec2D& playerPos = player.getPosition();
    for (auto& object : objects) {
        // determine if object can be seen by the player
        Vec2D difV(object->getPosition().getX() - playerPos.getX(), object->getPosition().getY() - playerPos.getY());
        float distanceFromPlayer = difV.magnitude();

        // determine if object is inside player's FOV
        camera.setX(sinf(player.getViewAngle()));
        camera.setY(cosf(player.getViewAngle()));
        float objAngle = atan2f(camera.getY(), camera.getX()) - atan2f(difV.getY(), difV.getX());
        if (objAngle < -PI_F) {
            objAngle += 2.0f * PI_F;
        }
        if (objAngle > PI_F) {
            objAngle -= 2.0f * PI_F;
        }
        bool inPlayerFov = fabs(objAngle) < (player.getFov() / 2.0f);
            
        // draw object (if necessary)
        if (inPlayerFov && distanceFromPlayer >= minObjectDrawDistance && distanceFromPlayer < rayDepth) {
            float objtCeilingY = (float)(ioh.getScreenHeight() / 2.0f) - ioh.getScreenHeight() / ((float)distanceFromPlayer);
            float objFloorY = ioh.getScreenHeight() - objtCeilingY;
            float objAspectRatio = (float)object->getSprite()->getHeight() / (float)object->getSprite()->getWidth();
            float objHeight = objFloorY - objtCeilingY;
            float objWidth = objHeight / objAspectRatio;
            float objMiddle = (0.5f * (objAngle / (player.getFov() / 2.0f)) + 0.5f) * (float)ioh.getScreenWidth();

            for (float ox = 0; ox < objWidth; ++ox) {
                for (float oy = 0; oy < objHeight; ++oy) {
                    float sampleX = ox / objWidth;
                    float sampleY = oy / objHeight;
                    wchar_t glyph = object->getSprite()->sampleGlyph(sampleX, sampleY);
                    int objColumn = (int)(objMiddle + ox - (objWidth / 2.0f));

                    if (objColumn >= 0 && objColumn < ioh.getScreenWidth() && objtCeilingY + oy >= 0 && objtCeilingY + oy < ioh.getScreenHeight()) {
                        if (glyph != Sprite::BLANK_GLYPH && depthBuffer[objColumn] >= distanceFromPlayer) {
                            ioh.drawGlyph(objColumn, (int)(objtCeilingY + oy), glyph, object->getSprite()->sampleColor(sampleX, sampleY));
                            depthBuffer[objColumn] = distanceFromPlayer;
                        }
                    }
                }
            }
        }
    }
}

void Renderer::showMinimap(const Map& map, const Vec2D& playerPos)
{
	for (int x = 0; x < map.getWidth(); ++x) {
		for (int y = 0; y < map.getHeight(); ++y) {
            ioh.drawGlyph(x, y, map.getCell(x, y));
		}
	}

	// show player on minimap
    ioh.drawGlyph((int)playerPos.getX(), (int)playerPos.getY(), map.getPlayerSymbol());
}

void Renderer::updateWindowTitle(const Player& player, float elapsedTime)
{
    static constexpr int MAX_LEN = 256; 
    wchar_t title[MAX_LEN];
    swprintf_s(title, MAX_LEN, L"CMD3DEngine - STATS: FPS=%3.2f, X=%3.2f, Y=%3.2f, A=%3.2f", 1.0 / elapsedTime,
        player.getPosition().getX(), player.getPosition().getY(), player.getViewAngle());
    ioh.setWindowTitle(title);
}