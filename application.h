#pragma once

#include "window.h"
#include "input.h"
#include "graphics.h"

#include "camera.h"

class object;
class submarine;
class cube;

class texture_shader;
class transparent_shader;
class light_shader;

class particle_emitter;
class light;

class application
{
public:
	application();
	~application();

	explicit application(application const&) = default;
	explicit application(application&&) = default;

	application& operator=(application const&) = default;
	application& operator=(application&&) = default;

	void update();
	void frame();
	void render();

	inline bool alive() const
	{ return alive_; }
	inline bool restart() const
	{ return restart_; }

private:
	bool alive_ = true;
	bool restart_ = false;

	using high_resolution_clock = std::chrono::high_resolution_clock;
	high_resolution_clock::time_point time_ = high_resolution_clock::now();
	std::chrono::microseconds delay_ = 15ms;

	// AntTweakBar
	TwBar *tweak_bar_ = nullptr;
	long long atb_delay_ = std::chrono::microseconds{ 15ms }.count();

	// camera
	std::shared_ptr<camera> camera_ = nullptr;

	// objects
	std::shared_ptr<submarine> submarine_ = nullptr;
	std::shared_ptr<cube> cube_ = nullptr;
	std::shared_ptr<object> inner_sphere_ = nullptr;
	std::shared_ptr<object> outer_sphere_ = nullptr;
	std::shared_ptr<object> terrain_ = nullptr;
	std::vector<std::shared_ptr<object>> fish_ = {};

	// shaders
	std::shared_ptr<texture_shader> texture_shader_ = nullptr;
	std::shared_ptr<transparent_shader> transparent_shader_ = nullptr;
	std::shared_ptr<light_shader> light_shader_ = nullptr;

	// engines
	std::unique_ptr<window> window_ = nullptr;
	std::unique_ptr<input> input_ = nullptr;
	std::unique_ptr<graphics> graphics_ = nullptr;

	// test
	std::vector<std::shared_ptr<light>> lights_ = {};
};