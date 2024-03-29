#include "pch.h"
#include "message_handler.h"

HINSTANCE const message_handler::instance_ = GetModuleHandle(NULL);

message_handler::message_handler(std::wstring const &name) : handle_(nullptr), name_(name)
{}

message_handler::~message_handler()
{
	UnregisterClass(name_.c_str(), instance_);
}

void message_handler::update()
{
	MSG msg{};
	while (PeekMessage(&msg, handle_.get(), 0U, 0U, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void message_handler::message(UINT const msg, WPARAM const w, LPARAM const l)
{}

LRESULT CALLBACK message_handler::hook(int const msg, WPARAM const w, LPARAM const l)
{
	if (HCBT_CREATEWND == msg) {
		auto const data = reinterpret_cast<LPCBT_CREATEWND>(l);

		SetWindowLongPtr(reinterpret_cast<HWND>(w), GWLP_USERDATA,
			reinterpret_cast<LONG_PTR>(data->lpcs->lpCreateParams));
	}

	return CallNextHookEx(nullptr, msg, w, l);
}

LRESULT CALLBACK message_handler::proc(HWND const wnd, UINT const msg, WPARAM const w, LPARAM const l)
{
	auto const pointer = GetWindowLongPtr(wnd, GWLP_USERDATA);
	auto const target = reinterpret_cast<message_handler*>(pointer);
	
	target->message(msg, w, l);

	return DefWindowProc(wnd, msg, w, l);
}