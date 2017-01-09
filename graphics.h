#pragma once

#include "direct3d.h"

class graphics
{
public:
	explicit graphics(HWND const&, std::shared_ptr<config> const&);

	graphics() = delete;
	~graphics() = default;

	explicit graphics(graphics const&) = default;
	explicit graphics(graphics&&) = default;

	graphics& operator=(graphics const&) = default;
	graphics& operator=(graphics&&) = default;

	void begin_render();
	void end_render();

	inline void toggle_atb()
	{ show_atb_ = !show_atb_; }
	inline void toggle_mode()
	{ d3d_.toggle_mode(); }

	inline direct3d const& d3d() const
	{ return d3d_; }

private:
	direct3d d3d_;
	bool show_atb_ = false;
};