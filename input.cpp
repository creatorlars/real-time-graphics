#include "pch.h"
#include "input.h"

input::input()
{
	auto const window_class = WNDCLASS
	{
		NULL,
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
		NULL,
		0, 0,
		0, 0,
		HWND_MESSAGE, NULL,
		instance_,
		this
	);

	UnhookWindowsHookEx(hook);

	auto const devices = std::array<RAWINPUTDEVICE, 2U>
	{{
		{ 0x01, 0x02, RIDEV_INPUTSINK, handle_ },	// mouse
		{ 0x01, 0x06, RIDEV_INPUTSINK, handle_ }	// keyboard
	}};

	RegisterRawInputDevices(devices.data(), devices.size(),
		sizeof(RAWINPUTDEVICE));
}

input::~input()
{
	DestroyWindow(handle_);
	handle_ = nullptr;

	UnregisterClass(class_name_, instance_);
	instance_ = nullptr;
	class_name_ = nullptr;
}

void input::update()
{
	MSG msg{};
	while (PeekMessage(&msg, handle_, 0U, 0U, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void input::message(UINT msg, WPARAM w, LPARAM l)
{
	if (msg != WM_INPUT) {
		return;
	}

	auto const raw = reinterpret_cast<HRAWINPUT>(l);
	auto size = UINT{};
	GetRawInputData(raw, RID_INPUT, NULL, &size, sizeof(RAWINPUTHEADER));
	auto data = std::make_unique<BYTE[]>(size);
	GetRawInputData(raw, RID_INPUT, data.get(), &size, sizeof(RAWINPUTHEADER));
	auto const input = reinterpret_cast<PRAWINPUT>(data.get());

	switch (input->header.dwType)
	{
	case RIM_TYPEKEYBOARD:
	{
		if (~input->data.keyboard.Flags & 0b01)
		{
			keys_[input->data.keyboard.VKey] = true;
		}
		else {
			keys_[input->data.keyboard.VKey] = false;
		}
		break;
	}
	case RIM_TYPEMOUSE:
	{
		break;
	}
	}
}