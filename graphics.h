#pragma once

#include "direct3d.h"

class graphics
{
public:
	graphics(HWND);
	~graphics() = default;

	void begin_render();
	void end_render();

	inline direct3d const& d3d() const { return d3d_; }

private:
	direct3d d3d_;
};