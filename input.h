#pragma once

#include "message_handler.h"

namespace KEYBOARD
{
	constexpr std::uint8_t
		ESCAPE = 0x1B, TAB = 0x09, R = 0x52, T = 0x54,
		LEFT = 0x25, UP = 0x26, RIGHT = 0x27, DOWN = 0x28,
		W = 0x57, A = 0x41, S = 0x53, D = 0x44,
		I = 0x49, J = 0x4A, K = 0x4B, L = 0x4C,
		PAGEUP = 0x21, PAGEDOWN = 0x22,
		SHIFT = 0x10, CTRL = 0x11,
		F1 = 0x70, F2 = 0x71, F3 = 0x72, F4 = 0x73, F5 = 0x74, F6 = 0x75;
}

namespace KEYSTATES
{
	// bit 0 - up/down flag
	// bit 1 - pressed/released flag
	// bit 2 - repeat flag
	constexpr std::uint8_t
		UP = 0b000,
		DOWN = 0b001,
		RELEASED = 0b010,
		PRESSED = 0b011,
		REPEAT = 0b101;
}

class input final : public message_handler
{
public:
	input();

	~input() = default;

	explicit input(input const&) = default;
	explicit input(input&&) = default;

	input& operator=(input const&) = default;
	input& operator=(input&&) = default;

	void update() override;

	inline std::uint8_t get(std::uint8_t const key) const
	{ return keys_.at(static_cast<size_t>(key)); }
	inline bool down(std::uint8_t const key) const
	{ return keys_.at(static_cast<size_t>(key)) & 0b1; }
	inline bool up(std::uint8_t const key) const
	{ return ~keys_.at(static_cast<size_t>(key)) & 0b1; }
	inline bool pressed(std::uint8_t const key) const
	{ return keys_.at(static_cast<size_t>(key)) == KEYSTATES::PRESSED; }
	inline bool released(std::uint8_t const key) const
	{ return keys_.at(static_cast<size_t>(key)) == KEYSTATES::RELEASED; }
	inline bool repeat(std::uint8_t const key) const
	{ return keys_.at(static_cast<size_t>(key)) == KEYSTATES::REPEAT; }
	inline bool trigger(std::uint8_t const key) const
	{ return pressed(key) || repeat(key); }

private:
	void message(UINT const, WPARAM const, LPARAM const) override;

	std::array<std::uint8_t, 256U> keys_ = {};
};
