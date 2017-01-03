#include "pch.h"
#include "window.h"

window::window() : message_handler(L"window")
{
	auto const window_class = WNDCLASS
	{
		CS_HREDRAW | CS_VREDRAW | CS_OWNDC,
		message_handler::proc,
		0, 0,
		instance_,
		NULL, NULL, NULL,
		NULL, name_
	};
	RegisterClass(&window_class);

	auto const hook = SetWindowsHookEx(WH_CBT, message_handler::hook, NULL,
		GetCurrentThreadId());

	handle_ = CreateWindow
	(
		window_class.lpszClassName, NULL,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VISIBLE,
		0, 0,
		WINDOW_WIDTH, WINDOW_HEIGHT,
		NULL, NULL,
		instance_,
		this
	);

	UnhookWindowsHookEx(hook);
}

void window::message(UINT msg, WPARAM w, LPARAM l)
{
	if (WM_DESTROY == msg)
	{
		alive_ = false;
		return;
	}

	TwEventWin(handle_, msg, w, l);
}