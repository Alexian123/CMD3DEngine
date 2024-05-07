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
    checkCoords(x, y);
    return world[y * width + x];
}

void Map::setCell(int x, int y, wchar_t val)
{
    checkCoords(x, y);
    world[y * width + x] = val;
}

bool Map::isWall(int x, int y) const
{
    checkCoords(x, y);
    return world[y * width + x] == wall;
}

wchar_t Map::getPlayerSymbol() const
{
    return player;
}

void Map::checkCoords(int x, int y) const
{
    if (x < 0 || x >= width || y < 0 || y >= height) {
        throw std::out_of_range("Map coordinates out of bounds");
    }
}