#pragma once

#include "object.h"

class cube final : public object
{
public:
	explicit cube(direct3d const&);

	cube() = delete;
	~cube() = default;

	explicit cube(cube const&) = default;
	explicit cube(cube&&) = default;

	cube& operator=(cube const&) = default;
	cube& operator=(cube&&) = default;

	void frame() override;
	void render() override;

private:
	direct3d const& d3d_;

	float rotation_ = 0.f;
};