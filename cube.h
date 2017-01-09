#pragma once

#include "object.h"

class direct3d;
class light_shader;

class cube final : public object
{
public:
	explicit cube(direct3d const&, std::shared_ptr<light_shader> const&);

	cube() = delete;
	virtual ~cube();

	explicit cube(cube const&) = default;
	explicit cube(cube&&) = default;

	cube& operator=(cube const&) = default;
	cube& operator=(cube&&) = default;

	void frame() override;
	void render(std::shared_ptr<camera> const &) const override;

private:
	direct3d const& d3d_;
	std::shared_ptr<light_shader> shader_ = nullptr;
};