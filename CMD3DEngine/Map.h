#pragma once

#include <string>

namespace CMD_3D_ENGINE
{
	class Map
	{
	private:
		const int width;
		const int height;
		const wchar_t wall;
		const wchar_t empty;
		const wchar_t player;
		std::wstring world;
	public:
		Map();
		Map(int width, int height, wchar_t wall, wchar_t empty, wchar_t player, std::wstring world);

		int getWidth() const;
		int getHeight() const;
		wchar_t getCell(int x, int y) const;
		void setCell(int x, int y, wchar_t val);

		bool isWall(int x, int y) const; // out of bounds == wall

		wchar_t getPlayerSymbol() const;
	private:
		bool checkCoords(int x, int y) const;
	};
}

