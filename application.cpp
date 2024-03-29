#include "pch.h"
#include "application.h"

#include "window.h"
#include "graphics.h"
#include "input.h"

#include "texture_shader.h"
#include "transparent_shader.h"
#include "light_shader.h"

#include "camera.h"

#include "object.h"
#include "submarine.h"
#include "drebbel.h"
#include "sphere.h"
#include "cube.h"
#include "terrain.h"
#include "fish.h"

#include "ambient.h"
#include "spotlight.h"

#include "config.h"

application::application() : alive_(true), restart_(false), focus_(), preset_selection_(0U)
{
	auto const settings = std::make_shared<config>(L"./config.ini");

	// initialise engines
	window_ = std::make_unique<window>(settings);
	graphics_ = std::make_unique<graphics>(window_->handle(), settings);
	input_ = std::make_unique<input>();

	// initialise shaders
	texture_shader_ = std::make_shared<texture_shader>(graphics_->d3d());
	transparent_shader_ = std::make_shared<transparent_shader>(graphics_->d3d());
	light_shader_ = std::make_shared<light_shader>(graphics_->d3d());

	// initialise camera
	auto const c1px = settings->read<float>(L"camera", L"camera1positionx");
	auto const c1py = settings->read<float>(L"camera", L"camera1positiony");
	auto const c1pz = settings->read<float>(L"camera", L"camera1positionz");
	auto const c1rx = settings->read<float>(L"camera", L"camera1rotationx");
	auto const c1ry = settings->read<float>(L"camera", L"camera1rotationy");
	auto const c1rz = settings->read<float>(L"camera", L"camera1rotationz");
	camera1position_ = { c1px, c1py, c1pz };
	camera1rotation_ = { c1rx, c1ry, c1rz };

	auto const c2px = settings->read<float>(L"camera", L"camera2positionx");
	auto const c2py = settings->read<float>(L"camera", L"camera2positiony");
	auto const c2pz = settings->read<float>(L"camera", L"camera2positionz");
	auto const c2rx = settings->read<float>(L"camera", L"camera2rotationx");
	auto const c2ry = settings->read<float>(L"camera", L"camera2rotationy");
	auto const c2rz = settings->read<float>(L"camera", L"camera2rotationz");
	camera2position_ = { c2px, c2py, c2pz };
	camera2rotation_ = { c2rx, c2ry, c2rz };

	camera_ = std::make_shared<camera>();
	camera_->position(camera1position_);
	camera_->rotation(camera1rotation_);

	// initialise objects
	const auto object_count = settings->read<unsigned>(L"objects", L"objectcount");
	objects_.reserve(object_count);

	for (auto i = 0U; i < object_count; ++i)
	{
		auto object = std::shared_ptr<::object>();

		auto const key = std::wstring{ L"object" } + std::to_wstring(i + 1);
		const auto type = settings->read(L"objects", key + L"type");
		if (type == L"submarine")
		{ object = std::make_shared<submarine>(graphics_->d3d(), light_shader_); }
		else if (type == L"drebbel")
		{ object = std::make_shared<drebbel>(graphics_->d3d(), light_shader_); }
		else if (type == L"cube")
		{ object = std::make_shared<cube>(graphics_->d3d(), light_shader_); }
		else if (type == L"terrain")
		{ object = std::make_shared<terrain>(graphics_->d3d(), light_shader_); }
		else if (type == L"sphere")
		{ object = std::make_shared<sphere>(graphics_->d3d(), transparent_shader_); }
		else if (type == L"fish")
		{ 
			object = std::make_shared<fish>(graphics_->d3d(),
				transparent_shader_, light_shader_);
		}
		else
		{ continue; }

		const auto px = settings->read<float>(L"objects", key + L"positionx");
		const auto py = settings->read<float>(L"objects", key + L"positiony");
		const auto pz = settings->read<float>(L"objects", key + L"positionz");
		object->position({ px, py, pz });

		const auto rx = settings->read<float>(L"objects", key + L"rotationx");
		const auto ry = settings->read<float>(L"objects", key + L"rotationy");
		const auto rz = settings->read<float>(L"objects", key + L"rotationz");
		object->rotation({ rx, ry, rz });

		const auto sx = settings->read<float>(L"objects", key + L"scalex");
		const auto sy = settings->read<float>(L"objects", key + L"scaley");
		const auto sz = settings->read<float>(L"objects", key + L"scalez");
		object->scale({ sx, sy, sz });

		objects_.emplace_back(object);
	}

	// initialise lights
	auto const adx = settings->read<float>(L"lights", L"ambientdirectionx");
	auto const ady = settings->read<float>(L"lights", L"ambientdirectiony");
	auto const adz = settings->read<float>(L"lights", L"ambientdirectionz");

	auto const aminr = settings->read<float>(L"lights", L"ambientmincolourr");
	auto const aming = settings->read<float>(L"lights", L"ambientmincolourg");
	auto const aminb = settings->read<float>(L"lights", L"ambientmincolourb");

	auto const amaxr = settings->read<float>(L"lights", L"ambientmaxcolourr");
	auto const amaxg = settings->read<float>(L"lights", L"ambientmaxcolourg");
	auto const amaxb = settings->read<float>(L"lights", L"ambientmaxcolourb");

	ambient_ = std::make_shared<ambient>(
		XMFLOAT3{ adx, ady, adz },
		XMFLOAT3{ aminr, aming, aminb },
		XMFLOAT3{ amaxr, amaxg, amaxb }
	);

	const auto light_count = settings->read<unsigned>(L"lights", L"lightcount");
	lights_.reserve(light_count);

	for (auto i = 0U; i < light_count; ++i)
	{
		auto const key = std::wstring{ L"light" } + std::to_wstring(i + 1);

		const auto px = settings->read<float>(L"lights", key + L"positionx");
		const auto py = settings->read<float>(L"lights", key + L"positiony");
		const auto pz = settings->read<float>(L"lights", key + L"positionz");

		const auto cr = settings->read<float>(L"lights", key + L"colourr");
		const auto cg = settings->read<float>(L"lights", key + L"colourg");
		const auto cb = settings->read<float>(L"lights", key + L"colourb");

		lights_.emplace_back(std::make_shared<spotlight>(
			XMFLOAT3{ px, py, pz },
			XMFLOAT3{ cr, cg, cb }
		));
	}
	lights_.shrink_to_fit();

	const auto preset_count = settings->read<unsigned>(L"lights", L"presetcount");
	presets_.reserve(preset_count + 1);
	presets_.emplace_back(1.f, 1.f); // default

	for (auto i = 0U; i < preset_count; ++i)
	{
		auto const key = std::wstring{ L"preset" } + std::to_wstring(i + 1);

		const auto exposure = settings->read<float>(L"lights", key + L"exposure");
		const auto gamma = settings->read<float>(L"lights", key + L"gamma");

		presets_.emplace_back(std::pair<float, float>(exposure, gamma));
	}
	presets_.shrink_to_fit();

	// initialise AntTweakBar
	tweak_bar_ = TwNewBar("AntTweakBar");
	TwAddVarRW(tweak_bar_, "Frame Delay", TW_TYPE_INT32, &atb_delay_,
		"min=1000 max=100000 step=100");

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
			camera_->move_forward(.1f);
		}
		else
		{
			camera_->rotate_forwards(1.f);
		}
	}
	if (input_->down(KEYBOARD::DOWN) || input_->down(KEYBOARD::S)
		|| input_->down(KEYBOARD::K))
	{
		if (input_->down(KEYBOARD::CTRL))
		{
			camera_->move_backward(.1f);
		}
		else
		{
			camera_->rotate_backwards(1.f);
		}
	}
	if (input_->down(KEYBOARD::LEFT) || input_->down(KEYBOARD::A)
		|| input_->down(KEYBOARD::J))
	{
		if (input_->down(KEYBOARD::CTRL))
		{
			camera_->move_left(.1f);
		}
		else
		{
			camera_->rotate_left(1.f);
		}
	}
	if (input_->down(KEYBOARD::RIGHT) || input_->down(KEYBOARD::D)
		|| input_->down(KEYBOARD::L))
	{
		if (input_->down(KEYBOARD::CTRL))
		{
			camera_->move_right(.1f);
		}
		else
		{
			camera_->rotate_right(1.f);
		}
	}
	if (input_->down(KEYBOARD::PAGEUP) && input_->down(KEYBOARD::CTRL))
	{
		camera_->move_up(.1f);
	}
	if (input_->down(KEYBOARD::PAGEDOWN) && input_->down(KEYBOARD::CTRL))
	{
		camera_->move_down(.1f);
	}

	// camera switching
	if (input_->pressed(KEYBOARD::F1))
	{
		camera_->position(camera1position_);
		camera_->rotation(camera1rotation_);
	}
	else if (input_->pressed(KEYBOARD::F2))
	{
		camera_->position(camera2position_);
		camera_->rotation(camera2rotation_);
	}
	else if (input_->pressed(KEYBOARD::F3) || input_->pressed(KEYBOARD::F4))
	{
		// switch camera focus
		if (input_->pressed(KEYBOARD::F4))
		{
			++focus_;
			if (focus_ == objects_.size())
			{
				focus_ = 0U;
			}
		}

		auto const object = objects_[focus_];
		auto const position = object->position();
		camera_->position(position);
		camera_->rotation({ 45.f, 0.f, 0.f });
		camera_->move_backward(5.f);
	}

	// render mode control
	if (input_->pressed(KEYBOARD::F5))
	{
		graphics_->toggle_mode();
	}

	// change lighting presets
	if (input_->pressed(KEYBOARD::F6))
	{
		++preset_selection_;
		if (preset_selection_ == presets_.size())
		{
			preset_selection_ = 0U;
		}
	}

	for (auto const &it : objects_)
	{
		it->frame();
	}
}

void application::render()
{
	graphics_->begin_render();
	auto const preset = presets_[preset_selection_];
	light_shader_->begin_render(ambient_, lights_, preset.first, preset.second);

	for (auto const &it : objects_)
	{
		it->render(camera_);
	}

	graphics_->end_render();
}