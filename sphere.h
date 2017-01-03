#pragma once

#include "object.h"

class direct3d;

class sphere final : public object
{
public:
	sphere(direct3d const&);

	void frame() override;
	void render() override;

private:
	direct3d const& d3d_;
};