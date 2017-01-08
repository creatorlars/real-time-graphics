#pragma once

#include "light.h"

class ambient final : public light
{
public:
	ambient(XMFLOAT3 const&, XMFLOAT4 const&, XMFLOAT4 const&);

	ambient() = delete;
	~ambient() = default;

	explicit ambient(ambient const&) = default;
	explicit ambient(ambient&&) = default;

	ambient& operator=(ambient const&) = default;
	ambient& operator=(ambient&&) = default;

	inline XMFLOAT3 const& direction() const
	{ return direction_; }
	
	inline void direction(XMFLOAT3 const &direction)
	{ direction_ = direction; }

private:
	XMFLOAT3 direction_ = {};
};
