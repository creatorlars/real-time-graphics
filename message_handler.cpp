#include "pch.h"
#include "message_handler.h"

message_handler::message_handler(LPCWSTR name) : name_(name)
{}

message_handler::~message_handler()
{
	DestroyWindow(handle_);
	handle_ = nullptr;

	UnregisterClass(name_, instance_);
}

void message_handler::update()
{
	MSG msg{};
	while (PeekMessage(&msg, handle_, 0U, 0U, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

LRESULT CALLBACK message_handler::hook(int msg, WPARAM w, LPARAM l)
{
	if (HCBT_CREATEWND == msg) {
		auto const data = reinterpret_cast<LPCBT_CREATEWND>(l);

		SetWindowLongPtr(reinterpret_cast<HWND>(w), GWLP_USERDATA,
			reinterpret_cast<LONG_PTR>(data->lpcs->lpCreateParams));
	}

	return CallNextHookEx(NULL, msg, w, l);
}

LRESULT CALLBACK message_handler::proc(HWND wnd, UINT msg, WPARAM w, LPARAM l)
{
	auto const pointer = GetWindowLongPtr(wnd, GWLP_USERDATA);
	auto const target = reinterpret_cast<message_handler*>(pointer);
	
	target->message(msg, w, l);

	return DefWindowProc(wnd, msg, w, l);
}