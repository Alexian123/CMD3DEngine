#include "Map.h"

#include <stdexcept>

using namespace CMD_3D_ENGINE;

Map::Map() : width(16), height(16), wall('#'), empty('.'), player('@')
{
    // default 16x16 empty map
    world += L"################";
    world += L"#..............#";
    world += L"#..............#";
    world += L"#..............#";
    world += L"#..............#";
    world += L"#..............#";
    world += L"#..............#";
    world += L"#..............#";
    world += L"#..............#";
    world += L"#..............#";
    world += L"#..............#";
    world += L"#..............#";
    world += L"#..............#";
    world += L"#..............#";
    world += L"#..............#";
    world += L"################";
}

Map::Map(int width, int height, wchar_t wall, wchar_t empty, wchar_t player, std::wstring world)
    : width(width), height(height), wall(wall), empty(empty), player(player), world(world)
{
}

int Map::getWidth() const
{
    return width;
}

int Map::getHeight() const
{
    return height;
}

wchar_t Map::getCell(int x, int y) const
{
    if (checkCoords(x, y)) {
        return world[y * width + x];
    }
    return L'\0';
}

void Map::setCell(int x, int y, wchar_t c)
{
    if (checkCoords(x, y)) {
        world[y * width + x] = c;
    }
}

bool Map::isWall(int x, int y) const
{
    if (checkCoords(x, y)) {
        return world[y * width + x] == wall;
    }
    return true;
}

wchar_t Map::getPlayerSymbol() const
{
    return player;
}

bool Map::checkCoords(int x, int y) const
{
    return x >= 0 && x < width && y >= 0 && y < height;
}