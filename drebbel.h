#pragma once

#include "object.h"

class direct3d;
class light_shader;

class drebbel final : public object
{
public:
	explicit drebbel(direct3d const&, std::shared_ptr<light_shader> const&);

	drebbel() = delete;
	~drebbel() = default;

	explicit drebbel(drebbel const&) = default;
	explicit drebbel(drebbel&&) = default;

	drebbel& operator=(drebbel const&) = default;
	drebbel& operator=(drebbel&&) = default;

	void frame() override;
	inline void render(std::shared_ptr<camera> const &camera) const override
	{ return object::render(shader_, camera); }

private:
	direct3d const& d3d_;
	std::shared_ptr<light_shader> shader_ = nullptr;
};