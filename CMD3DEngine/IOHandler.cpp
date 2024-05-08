#include "IOHandler.h"

#include <stdexcept>

using namespace CMD_3D_ENGINE;

IOHandler::IOHandler(int screenWidth, int screenHeight, int fontWidth, int fontHeight)
	: screenWidth(screenWidth), screenHeight(screenHeight), fontWidth(fontWidth), fontHeight(fontHeight), mousePos(0.0f, 0.0f)
{
	// save original handle
	originalConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	if (originalConsole == INVALID_HANDLE_VALUE) {
		error(L"Bad handle");
	}

	// create new console buffer
	consoleOut = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	if (consoleOut == INVALID_HANDLE_VALUE) {
		error(L"Bad handle");
	}

	// set visual size to minimum
	rectWindow = { 0, 0, 1, 1 };
	SetConsoleWindowInfo(consoleOut, TRUE, &rectWindow);

	// set screen buffer size
	COORD coord = { (short)screenWidth, (short)screenHeight };
	if (!SetConsoleScreenBufferSize(consoleOut, coord)) {
		error(L"SetConsoleScreenBufferSize");
	}

	// assign screen buffer to the console
	if (!SetConsoleActiveScreenBuffer(consoleOut)) {
		error(L"SetConsoleActiveScreenBuffer");
	}

	// set font size
	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	cfi.nFont = 0;
	cfi.dwFontSize.X = fontWidth;
	cfi.dwFontSize.Y = fontHeight;
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;

	// set font to Consolas
	wcscpy_s(cfi.FaceName, L"Consolas");
	if (!SetCurrentConsoleFontEx(consoleOut, false, &cfi)) {
		error(L"SetCurrentConsoleFontEx");
	}

	// get screen buffer info and check the maximum allowed window size
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if (!GetConsoleScreenBufferInfo(consoleOut, &csbi)) {
		error(L"GetConsoleScreenBufferInfo");
	}
	if (screenHeight > csbi.dwMaximumWindowSize.Y) {
		error(L"Screen height / font height too large");
	}
	if (screenWidth > csbi.dwMaximumWindowSize.X) {
		error(L"Screen width / font width too large");
	}

	// set physical console window size
	rectWindow = { 0, 0, (short)(screenWidth - 1),  (short)(screenHeight - 1) };
	if (!SetConsoleWindowInfo(consoleOut, TRUE, &rectWindow)) {
		error(L"SetConsoleWindowInfo");
	}

	// allocate screen buffer
	buffer = new CHAR_INFO[screenWidth * screenHeight];
	for (int i = 0; i < screenWidth * screenHeight; ++i) {
		memset(&buffer[i], 0, sizeof(buffer[i]));
	}

	// get console input handle
	consoleIn = GetStdHandle(STD_INPUT_HANDLE);
	if (consoleIn == INVALID_HANDLE_VALUE) {
		error(L"Bad handle");
	}

	// enable mouse input		
	if (!SetConsoleMode(consoleIn, ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT)) {
		error(L"SetConsoleMode");
	}

	// init keyboard & mouse state
	for (int i = 0; i < NUM_KEYS; ++i) {
		keys[i].pressed = false;
		keys[i].released = false;
		keys[i].held = false;
	}
	for (int i = 0; i < NUM_MOUSE; ++i) {
		mouse[i].pressed = false;
		mouse[i].released = false;
		mouse[i].held = false;
	}
}

void IOHandler::draw(int x, int y, short c, short color)
{
	checkCoords(x, y);
	buffer[y * screenWidth + x].Char.UnicodeChar = c;
	buffer[y * screenWidth + x].Attributes = color;
}

void IOHandler::setWindowTitle(const wchar_t* title)
{
	if (title) {
		SetConsoleTitle(title);
	}
}

void IOHandler::updateScreenBuffer()
{
	WriteConsoleOutput(consoleOut, buffer, { (short)screenWidth, (short)screenHeight }, { 0,0 }, &rectWindow);
}

void IOHandler::readInput()
{
	getKeyboardInput();
	getMouseInput();
}

keyState IOHandler::getKeyState(KeyboardKey k) const
{
	return keys[k];
}

keyState IOHandler::getMouseState(MouseBtn m) const
{
	return mouse[m];
}

const Vec2D& IOHandler::getMousePos() const
{
	return mousePos;
}

bool IOHandler::isFocused() const
{
	return consoleIsFocused;
}

int IOHandler::getScreenWidth() const
{
	return screenWidth;
}

int IOHandler::getScreenHeight() const
{
	return screenHeight;
}

int IOHandler::getFontWidth() const
{
	return fontWidth;
}

int IOHandler::getFontenWidth() const
{
	return fontHeight;
}

void IOHandler::checkCoords(int x, int y) const
{
	if (x < 0 || x >= screenWidth || y < 0 || y >= screenHeight) {
		throw std::out_of_range("Invalid draw coordinates");
	}
}

void IOHandler::getKeyboardInput()
{
	for (int k = 0; k < NUM_KEYS; ++k) {
		keyNewState[k] = GetAsyncKeyState(k);;

		keys[k].pressed = false;
		keys[k].released = false;

		if (keyNewState[k] != keyOldState[k]) {
			if (keyNewState[k] & KEY_STATE_MASK) {
				keys[k].pressed = !keys[k].held;
				keys[k].held = true;
			}
			else {
				keys[k].released = true;
				keys[k].held = false;
			}
		}

		keyOldState[k] = keyNewState[k];
	}
}

void IOHandler::getMouseInput()
{
	// check for window events
	static constexpr int MAX_NUM_EVENTS = 32;
	INPUT_RECORD inBuf[MAX_NUM_EVENTS];
	DWORD events = 0;
	GetNumberOfConsoleInputEvents(consoleIn, &events);
	if (events > 0) {
		ReadConsoleInput(consoleIn, inBuf, events, &events);
	}

	// handle events
	for (DWORD i = 0; i < events; ++i) {
		switch (inBuf[i].EventType) {
			case FOCUS_EVENT: {
				consoleIsFocused = inBuf[i].Event.FocusEvent.bSetFocus;
			} break;

			case MOUSE_EVENT: {
				switch (inBuf[i].Event.MouseEvent.dwEventFlags) {
					case MOUSE_MOVED: {
						mousePos.setX(inBuf[i].Event.MouseEvent.dwMousePosition.X);
						mousePos.setY(inBuf[i].Event.MouseEvent.dwMousePosition.Y);
					} break;

					case 0: {
						for (int m = 0; m < NUM_MOUSE; ++m) {
							mouseNewState[m] = (inBuf[i].Event.MouseEvent.dwButtonState & (1 << m)) > 0;
						}
					} break;

					default: break;
				}
			} break;

			default: break;
		}
	}

	for (int m = 0; m < NUM_MOUSE; ++m) {
		mouse[m].pressed = false;
		mouse[m].released = false;

		if (mouseNewState[m] != mouseOldState[m]) {
			if (mouseNewState[m]) {
				mouse[m].pressed = true;
				mouse[m].held = true;
			}
			else {
				mouse[m].released = true;
				mouse[m].held = false;
			}
		}

		mouseOldState[m] = mouseNewState[m];
	}
}

void IOHandler::error(const wchar_t* msg) const
{
	static constexpr int MAX_BUF_LEN = 256;
	wchar_t buf[MAX_BUF_LEN];
	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), buf, MAX_BUF_LEN, NULL);
	SetConsoleActiveScreenBuffer(originalConsole);
	wprintf(L"ERROR: %s\n\t%s\n", msg, buf);
	exit(EXIT_FAILURE);
}