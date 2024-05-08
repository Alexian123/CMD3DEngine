#pragma once

#include <Windows.h>
#include <string>

#include "Vec2D.h"

namespace CMD_3D_ENGINE
{
	enum KeyboardKey
	{
		W_KEY = 'W', A_KEY = 'A', S_KEY = 'S', D_KEY = 'D',
		Q_KEY = 'Q', E_KEY = 'E'
	};

	enum MouseBtn
	{
		MOUSE_LEFT_BTN = 0, MOUSE_RIGHT_BTN = 1, MOUSE_MIDDLE_BTN = 2,
	};

	struct keyState {
		bool pressed;
		bool released;
		bool held;
	};

	class IOHandler
	{
	private:
		static constexpr unsigned short KEY_STATE_MASK = 0x8000;
		static constexpr unsigned char NUM_KEYS = 255;
		static constexpr unsigned char NUM_MOUSE = 5;

		const int screenWidth;
		const int screenHeight;
		const int fontWidth;
		const int fontHeight;

		HANDLE originalConsole;
		HANDLE consoleOut;
		HANDLE consoleIn;

		SMALL_RECT rectWindow;

		CHAR_INFO* buffer;

		keyState keys[NUM_KEYS];
		keyState mouse[NUM_MOUSE];

		short keyOldState[NUM_KEYS] = { 0 };
		short keyNewState[NUM_KEYS] = { 0 };
		bool mouseOldState[NUM_MOUSE] = { 0 };
		bool mouseNewState[NUM_MOUSE] = { 0 };

		Vec2D mousePos;
		bool consoleIsFocused = false;
	public:
		IOHandler(int screenWidth, int screenHeight, int fontWidth, int fontHeight);

		// output
		void draw(int x, int y, short c, short color = 0x000F);
		void setWindowTitle(const wchar_t* title);
		void updateScreenBuffer();

		// input
		void readInput();
		keyState getKeyState(KeyboardKey k) const;
		keyState getMouseState(MouseBtn m) const;
		const Vec2D& getMousePos() const;
		bool isFocused() const;

		// getters
		int getScreenWidth() const;
		int getScreenHeight() const;
		int getFontWidth() const;
		int getFontenWidth() const;
	
	private:
		void checkCoords(int x, int y) const;
		void getKeyboardInput();
		void getMouseInput();
		void error(const wchar_t* msg) const;
	};
}


