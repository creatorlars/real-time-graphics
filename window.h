#pragma once

#include "message_handler.h"

class window : public message_handler
{
public:
	window();

	~window() = default;

	explicit window(window const&) = default;
	explicit window(window&&) = default;

	window& operator=(window const&) = default;
	window& operator=(window&&) = default;

	inline bool const alive() const
	{ return alive_; }

private:
	bool alive_ = true;

	void message(UINT const, WPARAM const, LPARAM const) override;
};