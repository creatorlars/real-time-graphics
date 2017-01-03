#include "pch.h"
#include "application.h"

#include "texture_shader.h"
#include "transparent_shader.h"

#include "submarine.h"
#include "sphere.h"
#include "cube.h"

application::application()
{
	window_ = std::make_unique<window>();
	input_ = std::make_unique<input>();
	graphics_ = std::make_unique<graphics>(window_->handle());

	texture_shader_ = std::make_shared<texture_shader>(graphics_->d3d());
	transparent_shader_ = std::make_shared<transparent_shader>(graphics_->d3d());

	submarine_ = std::make_shared<submarine>(graphics_->d3d());
	sphere_ = std::make_shared<sphere>(graphics_->d3d());
	cube_ = std::make_shared<cube>(graphics_->d3d());

	camera_.position({ 0.f, 0.f, -15.f });
}

void application::update()
{
	window_->update();
	input_->update();

	frame();
	render();
}

void application::frame()
{
	// exit application
	if (input_->get(KEYS::ESCAPE) || !window_->alive())
	{
		alive_ = false;
		return;
	}

	// restart application
	if (input_->get(KEYS::R))
	{
		restart_ = true;
		alive_ = false;
		return;
	}

	if (input_->get(KEYS::T))
	{
		if (input_->get(KEYS::SHIFT))
		{
			// increase time factor
		}
		else
		{
			// decrease time factor
		}
	}

	// statistics display
	if (input_->get(KEYS::TAB))
	{
		// toggle statistics display
	}

	// camera movement
	if (input_->get(KEYS::UP) || input_->get(KEYS::W) || input_->get(KEYS::I))
	{
		if (input_->get(KEYS::CTRL))
		{
			camera_.move({ 0.f, 0.f, .1f }); // pan forwards
		}
		else
		{
			camera_.rotate({ -.1f, 0.f, 0.f }); // rotate up
		}
	}
	if (input_->get(KEYS::DOWN) || input_->get(KEYS::S) || input_->get(KEYS::K))
	{
		if (input_->get(KEYS::CTRL))
		{
			camera_.move({ 0.f, 0.f, -.1f }); // pan backwards
		}
		else
		{
			camera_.rotate({ .1f, 0.f, 0.f }); // rotate down
		}
	}
	if (input_->get(KEYS::LEFT) || input_->get(KEYS::A) || input_->get(KEYS::J))
	{
		if (input_->get(KEYS::CTRL))
		{
			camera_.move({ .1f, 0.f, 0.f }); // pan left
		}
		else
		{
			camera_.rotate({ 0.f, -.1f, 0.f }); // rotate left
		}
	}
	if (input_->get(KEYS::RIGHT) || input_->get(KEYS::D) || input_->get(KEYS::L))
	{
		if (input_->get(KEYS::CTRL))
		{
			camera_.move({ -.1f, 0.f, 0.f }); // pan right
		}
		else
		{
			camera_.rotate({ 0.f, .1f, 0.f }); // rotate right
		}
	}
	if (input_->get(KEYS::PAGEUP))
	{
		if (input_->get(KEYS::CTRL))
		{
			camera_.move({ 0.f, -.1f, 0.f }); // pan up
		}
		else
		{
			camera_.rotate({ 0.f, 0.f, -.1f }); // rotate clockwise
		}
	}
	if (input_->get(KEYS::PAGEDOWN))
	{
		if (input_->get(KEYS::CTRL))
		{
			camera_.move({ 0.f, .1f, 0.f }); // pan down
		}
		else
		{
			camera_.rotate({ 0.f, 0.f, .1f }); // rotate counter-clockwise
		}
	}

	// camera switching
	if (input_->get(KEYS::F1))
	{
		// select camera 1
	}
	else if (input_->get(KEYS::F2))
	{
		// select camera 2
	}
	else if (input_->get(KEYS::F3))
	{
		// select camera 3
	}

	// camera 3 control
	if (input_->get(KEYS::F4))
	{
		// switch camera 3 focus
	}

	// render mode control
	if (input_->get(KEYS::F5))
	{
		// switch render modes
	}

	// deferred rendering visualisations
	if (input_->get(KEYS::F6))
	{
		// switch visualisations
	}

	camera_.frame();

	submarine_->frame();
	sphere_->frame();
	cube_->frame();
}

void application::render()
{
	graphics_->begin_render();

	camera_.render();

	texture_shader_->render(submarine_, camera_);
	texture_shader_->render(cube_, camera_);

	transparent_shader_->render(sphere_, camera_, .5f);

	graphics_->end_render();
}