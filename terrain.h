#pragma once

#include "object.h"

class direct3d;
class light_shader;

class terrain final : public object
{
public:
	explicit terrain(direct3d const&, std::shared_ptr<light_shader> const&);

	terrain() = delete;
	virtual ~terrain();

	explicit terrain(terrain const&) = default;
	explicit terrain(terrain&&) = default;

	terrain& operator=(terrain const&) = default;
	terrain& operator=(terrain&&) = default;

	void frame() override;
	void render(std::shared_ptr<camera> const &camera) const override;

private:
	direct3d const& d3d_;
	std::shared_ptr<light_shader> shader_ = nullptr;
};
