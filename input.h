#pragma once

#include "message_handler.h"

enum KEYS : std::uint8_t
{
	ESCAPE = 0x1B,
	TAB = 0x09,
	LEFT = 0x25, UP, RIGHT, DOWN,
	PAGEUP = 0x21, PAGEDOWN,
	A = 0x41, B, C, D, E, F, G, H, I, J, K, L,
	M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
	SHIFT = 0x10, CTRL,
	F1 = 0x70, F2, F3, F4, F5, F6
};

class input final : private message_handler
{
public:
	input();
	~input();

	void update();

	inline bool get(KEYS key) const { return keys_.at(key); }

private:
	void message(UINT, WPARAM, LPARAM) override;

	std::array<bool, 256U> keys_;

	HWND handle_;
	HINSTANCE instance_ = GetModuleHandle(NULL);
	LPCWSTR class_name_ = L"INPUT";
};
