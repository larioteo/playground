﻿module;

#undef APIENTRY
#define NOMINMAX
#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "Ultra/Core.h"

module Platform.System.Input;


namespace Ultra {

bool WinInput::GetKeyStatePlatform(KeyCode code) const {
	return (bool)::GetAsyncKeyState((int)code);
}

bool WinInput::GetMouseButtonStatePlatform(MouseButton button) const {
	switch (button) {
		case MouseButton::Left:		return (bool)::GetAsyncKeyState((int)VK_LBUTTON);
		case MouseButton::Middle:	return (bool)::GetAsyncKeyState((int)VK_MBUTTON);
		case MouseButton::Right:	return (bool)::GetAsyncKeyState((int)VK_RBUTTON);
		case MouseButton::X1:		return (bool)::GetAsyncKeyState((int)VK_XBUTTON1);
		case MouseButton::X2:		return (bool)::GetAsyncKeyState((int)VK_XBUTTON2);
		default:					return false;
	}
}

std::pair<float, float> WinInput::GetMousePositionPlatform() const {
	POINT point;
	GetCursorPos(&point);
	ScreenToClient(GetActiveWindow(), &point);
	return { static_cast<float>(point.x), static_cast<float>(point.y) };
}

}
