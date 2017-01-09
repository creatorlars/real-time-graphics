#include "pch.h"
#include "graphics.h"

#include "direct3d.h"

class config;

graphics::graphics(HWND const& handle, std::shared_ptr<config> const &settings)
	: d3d_(handle, settings), show_atb_(false)
{}

void graphics::begin_render()
{
	d3d_.begin(0.f, 0.f, 0.f, 1.f);
	d3d_.enable_alpha_blending();
}

void graphics::end_render()
{
	if (show_atb_)
	{
		TwDraw();
	}
	
	d3d_.disable_alpha_blending();
	d3d_.end();
}