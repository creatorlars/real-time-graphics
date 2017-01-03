#pragma once

#include "message_handler.h"

class window : public message_handler
{
public:
	window();

	inline bool const alive() const { return alive_; }

private:
	bool alive_ = true;

	void message(UINT, WPARAM, LPARAM) override;
};