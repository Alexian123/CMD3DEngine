// CMD3DEngine.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <string>

#include "ScreenRenderer.h"
#include "KeyboardHandler.h"
#include "FrameTimer.h"
#include "Map.h"
#include "Player.h"

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