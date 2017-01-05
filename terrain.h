#pragma once

#include "object.h"

class direct3d;

class terrain final : public object
{
public:
	explicit terrain(direct3d const&);

	terrain() = delete;
	~terrain() = default;

	explicit terrain(terrain const&) = default;
	explicit terrain(terrain&&) = default;

	terrain& operator=(terrain const&) = default;
	terrain& operator=(terrain&&) = default;

private:
	direct3d const& d3d_;
};
