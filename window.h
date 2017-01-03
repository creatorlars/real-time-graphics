#pragma once

#include "message_handler.h"

class window : private message_handler
{
public:
	window();
	~window();

	void update();

	inline HWND const& handle() const { return handle_; }

private:
	HWND handle_ = nullptr;
	HINSTANCE instance_ = GetModuleHandle(NULL);
	LPCWSTR class_name_ = L"WINDOW";

	void message(UINT, WPARAM, LPARAM) override;
};