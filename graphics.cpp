#include "pch.h"
#include "graphics.h"

#include "direct3d.h"

graphics::graphics(HWND handle) :
	d3d_(WINDOW_WIDTH, WINDOW_HEIGHT, VSYNC_ENABLED, handle, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR)
{
}

void graphics::begin_render()
{
	d3d_.begin(0.f, 0.f, 0.f, 1.f);
	d3d_.enable_alpha_blending();
}

void graphics::end_render()
{
	d3d_.disable_alpha_blending();
	d3d_.end();
}