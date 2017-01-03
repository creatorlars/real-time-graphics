#include "pch.h"
#include "window.h"

window::window() : message_handler(L"window")
{
	auto const window_class = WNDCLASS
	{
		CS_HREDRAW | CS_VREDRAW | CS_OWNDC,
		message_handler::proc,
		0, 0,
		instance(),
		nullptr, nullptr, nullptr,
		nullptr, name()
	};
	RegisterClass(&window_class);

	auto const hook = SetWindowsHookEx(WH_CBT, message_handler::hook, nullptr,
		GetCurrentThreadId());

	handle(CreateWindow
	(
		window_class.lpszClassName, nullptr,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VISIBLE,
		0, 0,
		WINDOW_WIDTH, WINDOW_HEIGHT,
		nullptr, nullptr,
		instance(),
		this
	));

	UnhookWindowsHookEx(hook);
}

void window::message(UINT const msg, WPARAM const w, LPARAM const l)
{
	if (WM_DESTROY == msg)
	{
		alive_ = false;
		return;
	}

	TwEventWin(handle(), msg, w, l);
}