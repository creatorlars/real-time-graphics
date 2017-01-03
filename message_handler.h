#pragma once

class message_handler
{
protected:
	virtual void message(UINT, WPARAM, LPARAM) = 0;

	static LRESULT CALLBACK hook(int, WPARAM, LPARAM);
	static LRESULT CALLBACK proc(HWND, UINT, WPARAM, LPARAM);
};