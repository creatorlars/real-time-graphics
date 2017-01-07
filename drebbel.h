#pragma once

#include "object.h"

class direct3d;

class drebbel final : public object
{
public:
	explicit drebbel(direct3d const&);

	drebbel() = delete;
	~drebbel() = default;

	explicit drebbel(drebbel const&) = default;
	explicit drebbel(drebbel&&) = default;

	drebbel& operator=(drebbel const&) = default;
	drebbel& operator=(drebbel&&) = default;

	void frame();

private:
	direct3d const& d3d_;
};