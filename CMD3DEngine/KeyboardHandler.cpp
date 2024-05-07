#include "KeyboardHandler.h"

#include <Windows.h>

using namespace CMD_3D_ENGINE;

KeyboardHandler::KeyboardHandler()
{
}

bool KeyboardHandler::isKeyPressed(Key k) const
{
	return GetAsyncKeyState((unsigned short)k) & MASK;
}