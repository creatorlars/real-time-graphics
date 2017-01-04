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

	camera_ = std::make_shared<camera>();
	camera_->position({ 0.f, 0.f, -15.f });

	test_bar_ = TwNewBar("test_bar");

	timeBeginPeriod(1U);
}

application::~application()
{
	timeEndPeriod(1U);
}

void application::update()
{
	window_->update();
	input_->update();
	frame();

	auto const frame = std::chrono::high_resolution_clock::now();
	auto difference = frame - time_;
	while (difference < delay_)
	{
		render();
		Sleep(1);
		difference = std::chrono::high_resolution_clock::now() - time_;
	}

	time_ = frame;
}

void application::frame()
{
	// exit application
	if (input_->pressed(KEYBOARD::ESCAPE) || !window_->alive())
	{
		TwTerminate();
		alive_ = false;
		return;
	}

	// restart application
	if (input_->pressed(KEYBOARD::R))
	{
		restart_ = true;
		alive_ = false;
		return;
	}

	// increase / decrease frame time delay
	if (input_->pressed(KEYBOARD::T))
	{
		if (input_->down(KEYBOARD::SHIFT))
		{
			delay_ += 1ms;
		}
		else
		{
			if (delay_ > 1ms)
			{
				delay_ -= 1ms;
			}
		}
		std::cout << delay_.count() << '\n';
	}

	// statistics display
	if (input_->pressed(KEYBOARD::TAB))
	{
		graphics_->toggle_atb();
	}

	// camera movement
	if (input_->down(KEYBOARD::UP) || input_->down(KEYBOARD::W)
		|| input_->down(KEYBOARD::I))
	{
		if (input_->down(KEYBOARD::CTRL))
		{
			camera_->move_forward();
		}
		else
		{
			camera_->rotate_up();
		}
	}
	if (input_->down(KEYBOARD::DOWN) || input_->down(KEYBOARD::S)
		|| input_->down(KEYBOARD::K))
	{
		if (input_->down(KEYBOARD::CTRL))
		{
			camera_->move_backward();
		}
		else
		{
			camera_->rotate_down();
		}
	}
	if (input_->down(KEYBOARD::LEFT) || input_->down(KEYBOARD::A)
		|| input_->down(KEYBOARD::J))
	{
		if (input_->down(KEYBOARD::CTRL))
		{
			camera_->move_left();
		}
		else
		{
			camera_->rotate_left();
		}
	}
	if (input_->down(KEYBOARD::RIGHT) || input_->down(KEYBOARD::D)
		|| input_->down(KEYBOARD::L))
	{
		if (input_->down(KEYBOARD::CTRL))
		{
			camera_->move_right();
		}
		else
		{
			camera_->rotate_right();
		}
	}
	if (input_->down(KEYBOARD::PAGEUP) && input_->down(KEYBOARD::CTRL))
	{
		camera_->move_up();
	}
	if (input_->down(KEYBOARD::PAGEDOWN) && input_->down(KEYBOARD::CTRL))
	{
		camera_->move_down();
	}

	// camera switching
	if (input_->pressed(KEYBOARD::F1))
	{
		// select camera 1
	}
	else if (input_->pressed(KEYBOARD::F2))
	{
		// select camera 2
	}
	else if (input_->pressed(KEYBOARD::F3))
	{
		// select camera 3
	}

	// camera 3 control
	if (input_->pressed(KEYBOARD::F4))
	{
		// switch camera 3 focus
	}

	// render mode control
	if (input_->pressed(KEYBOARD::F5))
	{
		// switch render modes
	}

	// deferred rendering visualisations
	if (input_->pressed(KEYBOARD::F6))
	{
		// switch visualisations
	}

	submarine_->frame();
	sphere_->frame();
	cube_->frame();
}

void application::render()
{
	graphics_->begin_render();

	camera_->render();

	texture_shader_->render(submarine_, camera_);
	texture_shader_->render(cube_, camera_);

	transparent_shader_->render(sphere_, camera_, .5f);

	graphics_->end_render();
}