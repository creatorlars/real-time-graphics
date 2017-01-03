#include "pch.h"
#include "message_handler.h"

LRESULT CALLBACK message_handler::hook(int msg, WPARAM w, LPARAM l)
{
	if (msg == HCBT_CREATEWND) {
		auto const data = reinterpret_cast<LPCBT_CREATEWND>(l);

		SetWindowLongPtr(
			reinterpret_cast<HWND>(w),
			GWL_USERDATA,
			reinterpret_cast<LONG_PTR>(data->lpcs->lpCreateParams)
		);
	}

	return CallNextHookEx(NULL, msg, w, l);
}

LRESULT CALLBACK message_handler::proc(HWND wnd, UINT msg, WPARAM w, LPARAM l)
{
	auto const pointer = GetWindowLongPtr(wnd, GWL_USERDATA);
	auto const target = reinterpret_cast<message_handler*>(pointer);

	target->message(msg, w, l);

	return DefWindowProc(wnd, msg, w, l);
}