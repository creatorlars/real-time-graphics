#pragma once

class message_handler
{
public:
	explicit message_handler(std::wstring const&);
	virtual ~message_handler();

	message_handler() = delete;

	explicit message_handler(message_handler const&) = default;
	explicit message_handler(message_handler&&) = default;

	message_handler& operator=(message_handler const&) = default;
	message_handler& operator=(message_handler&&) = default;

	virtual void update();

	inline HWND const handle() const
	{ return handle_.get(); }
	inline std::wstring const& name() const
	{ return name_; }
	inline HINSTANCE const& instance() const
	{ return instance_; }

	inline void handle(HWND const& handle)
	{ handle_.reset(handle); }

protected:
	virtual void message(UINT const, WPARAM const, LPARAM const);

	static LRESULT CALLBACK hook(int const, WPARAM const, LPARAM const);
	static LRESULT CALLBACK proc(HWND, UINT, WPARAM, LPARAM);

private:
	struct HWND_DELETER
	{
		inline void operator()(HWND const p) const
		{ DestroyWindow(p); }
	};

	std::unique_ptr<HWND__, HWND_DELETER> handle_;
	std::wstring name_ = {};

	static HINSTANCE const instance_;
};