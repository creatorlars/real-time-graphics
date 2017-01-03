#pragma once

#include "window.h"
#include "input.h"
#include "graphics.h"

#include "camera.h"

class object;

class texture_shader;
class transparent_shader;

class application
{
public:
	application();
	~application() = default;

	explicit application(application const&) = default;
	explicit application(application&&) = default;

	application& operator=(application const&) = default;
	application& operator=(application&&) = default;

	void update();
	void frame();
	void render();

	inline bool alive() const { return alive_; }
	inline bool restart() const { return restart_; }

private:
	bool alive_ = true;
	bool restart_ = false;

	// AntTweakBar
	TwBar *test_bar_ = nullptr;

	// camera
	std::shared_ptr<camera> camera_ = nullptr;

	// objects
	std::shared_ptr<object> submarine_ = nullptr;
	std::shared_ptr<object> sphere_ = nullptr;
	std::shared_ptr<object> cube_ = nullptr;

	// shaders
	std::shared_ptr<texture_shader> texture_shader_ = nullptr;
	std::shared_ptr<transparent_shader> transparent_shader_ = nullptr;

	// engines
	std::unique_ptr<window> window_ = nullptr;
	std::unique_ptr<input> input_ = nullptr;
	std::unique_ptr<graphics> graphics_ = nullptr;
};