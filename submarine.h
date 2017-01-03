#pragma once

#include "object.h"

class direct3d;

class submarine final : public object
{
public:
	explicit submarine(direct3d const&);

	submarine() = default;
	~submarine() = default;

	explicit submarine(submarine const&) = default;
	explicit submarine(submarine&&) = default;

	submarine& operator=(submarine const&) = default;
	submarine& operator=(submarine&&) = default;

	void frame() override;
	void render() override;

private:
	direct3d const& d3d_;

	float rotation_ = 0.f;
};