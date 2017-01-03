#pragma once

class message_handler
{
public:
	explicit message_handler(LPCWSTR);
	virtual ~message_handler();

	explicit message_handler(message_handler const&) = default;
	explicit message_handler(message_handler&&) = default;

	message_handler& operator=(message_handler const&) = default;
	message_handler& operator=(message_handler&&) = default;

	virtual void update();

	inline HWND const& handle() const { return handle_; }
	inline LPCWSTR const& name() const { return name_; }
	inline HINSTANCE const& instance() const { return instance_; }

	inline void handle(HWND const& handle) { handle_ = handle; }

protected:
	virtual void message(UINT const, WPARAM const, LPARAM const);

	static LRESULT CALLBACK hook(int const, WPARAM const, LPARAM const);
	static LRESULT CALLBACK proc(HWND, UINT, WPARAM, LPARAM);

private:
	HINSTANCE const instance_ = GetModuleHandle(NULL);
	LPCWSTR name_ = nullptr;
	HWND handle_ = nullptr;
};