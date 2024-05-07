// CMD3DEngine.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <string>

#include "ScreenRenderer.h"
#include "KeyboardHandler.h"
#include "FrameTimer.h"
#include "Map.h"
#include "Player.h"

#define SCREEN_WIDTH 120
#define SCREEN_HEIGHT 40

float fPlayerX = 8.0f;
float fPlayerY = 8.0f;
float fPlayerA = 0.0f;
float fPlayerFOV = 3.14159f / 4.0f;
float fRayDepth = 16.0f;
float fPlayerRotateStep = 0.5f;
float fPlayerMovementStep = 5.0f;

const int nMapHeight = 16;
const int nMapWidth = 16;

using namespace CMD_3D_ENGINE;

int main() 
{
    // renderer init
    ScreenRenderer renderer(120, 40);

    // keyboard init
    KeyboardHandler khand;

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
    Player player(map, 8.0f, 8.0f, 0.0f);

    // game loop
    for (;;) {
        float elapsedTime = ft.update();
        player.handleControls(khand, elapsedTime);
        renderer.renderScene(player, map, elapsedTime);
    }
    
    return 0;
}