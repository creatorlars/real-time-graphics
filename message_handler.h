#pragma once

class message_handler
{
public:
	message_handler(LPCWSTR);
	virtual ~message_handler();

	void update();

	inline HWND const& handle() const { return handle_; }

protected:
	virtual void message(UINT, WPARAM, LPARAM) {};

	static LRESULT CALLBACK hook(int, WPARAM, LPARAM);
	static LRESULT CALLBACK proc(HWND, UINT, WPARAM, LPARAM);

	HINSTANCE const instance_ = GetModuleHandle(NULL);
	HWND handle_ = nullptr;
	LPCWSTR name_ = nullptr;
};