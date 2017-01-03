#include "pch.h"
#include "window.h"

window::window()
{
	auto const window_class = WNDCLASS
	{
		CS_HREDRAW | CS_VREDRAW | CS_OWNDC,
		message_handler::proc,
		0, 0,
		instance_,
		NULL, NULL, NULL,
		NULL, class_name_
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

window::~window()
{
	DestroyWindow(handle_);
	handle_ = nullptr;

	UnregisterClass(class_name_, instance_);
	instance_ = nullptr;
	class_name_ = nullptr;
}

void window::update()
{
	MSG msg{};
	while (PeekMessage(&msg, handle_, 0U, 0U, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void window::message(UINT msg, WPARAM w, LPARAM l)
{
}