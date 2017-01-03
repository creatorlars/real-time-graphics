#pragma once

#include "object.h"

class direct3d;

class sphere final : public object
{
public:
	explicit sphere(direct3d const&);

	sphere() = default;
	~sphere() = default;

	explicit sphere(sphere const&) = default;
	explicit sphere(sphere&&) = default;

	sphere& operator=(sphere const&) = default;
	sphere& operator=(sphere&&) = default;

	void frame() override;
	void render() override;

private:
	direct3d const& d3d_;
};