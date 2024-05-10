// CMD3DEngine.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <string>

#include "IOHandler.h"
#include "Renderer.h"
#include "FrameTimer.h"
#include "Map.h"
#include "Player.h"
#include "Object.h"

using namespace CMD_3D_ENGINE;

int main() 
{
    // IO init
    IOHandler ioh(320, 190, 4, 4);

    // renderer init
    Renderer renderer(ioh, L"./resources/sprites/wall.spr");

    // timer init
    FrameTimer ft;

    // map init
    std::wstring world;
    world += L"################";
    world += L"#....#.........#";
    world += L"#....#.........#";
    world += L"#....#.........#";
    world += L"#....#.........#";
    world += L"#....#.........#";
    world += L"#....####....###";
    world += L"#....#.........#";
    world += L"#....#.........#";
    world += L"#....#.........#";
    world += L"#....#.........#";
    world += L"#..............#";
    world += L"#..............#";
    world += L"#.........######";
    world += L"#..............#";
    world += L"################";
    Map map(16, 16, '#', '.', '@', world);

    // player init
    Player player(Vec2D(8.0f, 8.0f), map, -3.14159f / 2.0f);

    // objects init
    Object lamp1(Vec2D(8, 9), L"./resources/sprites/lamp.spr");
    Object lamp2(Vec2D(8, 12), lamp1.getSprite());
    Object lamp3(Vec2D(10, 3), lamp1.getSprite());

    // game loop
    for (;;) {
        float elapsedTime = ft.update();
        player.handleControls(ioh, elapsedTime);
        renderer.renderScene(player, map, elapsedTime);
    }
    
    return 0;
}