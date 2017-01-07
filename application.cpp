#include "pch.h"
#include "application.h"

#include "texture_shader.h"
#include "transparent_shader.h"
#include "light_shader.h"

#include "submarine.h"
#include "sphere.h"
#include "cube.h"
#include "terrain.h"
#include "fish.h"

#include "particle_emitter.h"
#include "light.h"

application::application()
{
	window_ = std::make_unique<window>();
	input_ = std::make_unique<input>();
	graphics_ = std::make_unique<graphics>(window_->handle());

	texture_shader_ = std::make_shared<texture_shader>(graphics_->d3d());
	transparent_shader_ = std::make_shared<transparent_shader>(graphics_->d3d());
	light_shader_ = std::make_shared<light_shader>(graphics_->d3d());

	submarine_ = std::make_shared<submarine>(graphics_->d3d());
	submarine_->translate({ 0.f, 1.f, 0.f });
	submarine_->scale({ .5f, .5f, .5f });

	cube_ = std::make_shared<cube>(graphics_->d3d());
	cube_->translate({ 5.f, 0.f, 0.f });

	inner_sphere_ = std::make_shared<sphere>(graphics_->d3d());
	inner_sphere_->scale({ -5, -5, -5 });

	outer_sphere_ = std::make_shared<sphere>(graphics_->d3d());
	outer_sphere_->scale({ 5, 5, 5.f });

	terrain_ = std::make_shared<terrain>(graphics_->d3d());
	terrain_->scale({ 4.99f, 4.99f, 4.99f });

	camera_ = std::make_shared<camera>();
	camera_->rotation({ 40.f, 0.f, 0.f });
	camera_->move({ 0.f, 0.f, -15.f });

	tweak_bar_ = TwNewBar("AntTweakBar");
	TwAddVarRW(tweak_bar_, "Frame Delay", TW_TYPE_INT32, &atb_delay_,
		"min=1000 max=100000 step=100");

	for (auto i = 0U; i < 9U; ++i)
	{
		fish_.emplace_back(std::make_shared<fish>(graphics_->d3d(), camera_));
		fish_.at(i)->scale({ .1f, .1f, .1f });
		fish_.at(i)->translate({ static_cast<float>(i) - 4.f, 2.f, 0.f });
	}

	lights_.emplace_back(std::make_shared<light>(XMFLOAT4{ .5f, 0.f, 0.f, 0.f }));
	lights_.emplace_back(std::make_shared<light>(XMFLOAT4{ 0.f, .5f, 0.f, 0.f }));

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
	delay_ = std::chrono::microseconds{ atb_delay_ };

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
	if (input_->trigger(KEYBOARD::T))
	{
		if (input_->down(KEYBOARD::SHIFT))
		{
			delay_ += 100us;
		}
		else
		{
			if (delay_ > 1ms)
			{
				delay_ -= 100us;
			}
		}

		atb_delay_ = delay_.count();
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
	cube_->frame();

	for (auto const& fish : fish_)
	{
		fish->frame();
	}
}

void application::render()
{
	graphics_->begin_render();

	camera_->render();

	light_shader_->render(submarine_, camera_, lights_);
	light_shader_->render(cube_, camera_, lights_);
	light_shader_->render(terrain_, camera_, lights_);

	transparent_shader_->render(inner_sphere_, camera_, .25f);

	for (auto const& fish : fish_)
	{
		fish->render(texture_shader_, camera_);
		light_shader_->render(fish, camera_, lights_);
	}

	transparent_shader_->render(outer_sphere_, camera_, .25f);

	graphics_->end_render();
}