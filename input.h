#pragma once

#include "message_handler.h"

enum KEYBOARD : std::uint8_t
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

class input final : public message_handler
{
public:
	input();

	void update() override;

	inline std::uint8_t get(std::uint8_t const key) const { return keys_.at(key); }
	inline bool down(std::uint8_t const key) const { return keys_.at(key) & 0b01; }
	inline bool up(std::uint8_t const key) const { return !down(key); }
	inline bool pressed(std::uint8_t const key) const { return keys_.at(key) == 0b11; }
	inline bool released(std::uint8_t const key) const { return keys_.at(key) == 0b10; }

private:
	void message(UINT const, WPARAM const, LPARAM const) override;

	std::array<std::uint8_t, 256U> keys_ = {};
};
