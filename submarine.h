#pragma once

#include "object.h"

class direct3d;
class light_shader;

class submarine final : public object
{
public:
	explicit submarine(direct3d const&, std::shared_ptr<light_shader> const&);

	submarine() = delete;
	~submarine() = default;

	explicit submarine(submarine const&) = default;
	explicit submarine(submarine&&) = default;

	submarine& operator=(submarine const&) = default;
	submarine& operator=(submarine&&) = default;

	void frame();
	inline void render(std::shared_ptr<camera> const &camera) const override
	{ return object::render(shader_, camera); }

private:
	direct3d const& d3d_;
	std::shared_ptr<light_shader> shader_ = nullptr;
};