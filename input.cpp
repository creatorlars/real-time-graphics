#include "pch.h"
#include "input.h"

input::input() : message_handler(L"input")
{
	auto const window_class = WNDCLASS
	{
		0U,
		message_handler::proc,
		0, 0,
		instance(),
		nullptr, nullptr, nullptr,
		nullptr, name().c_str()
	};
	RegisterClass(&window_class);

	auto const hook = SetWindowsHookEx(WH_CBT, message_handler::hook, nullptr,
		GetCurrentThreadId());

	constexpr auto hwnd_message = reinterpret_cast<HWND>(-3);
	handle(CreateWindow
	(
		window_class.lpszClassName, nullptr,
		0UL,
		0, 0,
		0, 0,
		hwnd_message, nullptr,
		instance(),
		this
	));

	UnhookWindowsHookEx(hook);

	constexpr auto usage_page = 0x01;
	constexpr auto mouse_usage = 0x02;
	constexpr auto keyboard_usage = 0x06;
	auto const devices = std::array<RAWINPUTDEVICE, 2U>
	{{
		{ usage_page, mouse_usage, RIDEV_INPUTSINK, handle() },
		{ usage_page, keyboard_usage, RIDEV_INPUTSINK, handle() }
	}};

	RegisterRawInputDevices(devices.data(), static_cast<UINT>(devices.size()),
		sizeof(RAWINPUTDEVICE));
}

void input::update()
{
	for (auto &it : keys_)
	{
		// clear flags 
		it &= 0b001;
	}

	message_handler::update();
}

void input::message(UINT const msg, WPARAM const w, LPARAM const l)
{
	if (WM_INPUT != msg) {
		return;
	}

	auto const raw = reinterpret_cast<HRAWINPUT>(l);
	auto size = UINT{};
	GetRawInputData(raw, RID_INPUT, nullptr, &size, sizeof(RAWINPUTHEADER));
	auto data = std::make_unique<BYTE[]>(size);
	GetRawInputData(raw, RID_INPUT, data.get(), &size, sizeof(RAWINPUTHEADER));

	auto const input = reinterpret_cast<PRAWINPUT>(data.get());
	switch (input->header.dwType)
	{
	case RIM_TYPEKEYBOARD:
	{
		// check if key is being released (highest bit set)
		if (input->data.keyboard.Flags & 0b1)
		{
			keys_[input->data.keyboard.VKey] = KEYSTATES::RELEASED;
		}
		else {
			// check if the key is already held down
			if (~keys_[input->data.keyboard.VKey] & 0b1)
			{
				keys_[input->data.keyboard.VKey] = KEYSTATES::PRESSED;
			}
			else
			{
				keys_[input->data.keyboard.VKey] = KEYSTATES::REPEAT;
			}
		}
		break;
	}
	case RIM_TYPEMOUSE:
	{
		break;
	}
	}
}