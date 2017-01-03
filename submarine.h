#pragma once

#include "object.h"

class direct3d;

class submarine final : public object
{
public:
	submarine(direct3d const&);

	void frame() override;
	void render() override;

private:
	direct3d const& d3d_;

	float rotation_ = 0.f;
};