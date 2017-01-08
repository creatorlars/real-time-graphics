#pragma once

#include "light.h"

class spotlight : public light
{
public:
	spotlight(XMFLOAT3 const&, XMFLOAT4 const&, XMFLOAT4 const&);

	spotlight() = delete;
	~spotlight() = default;

	explicit spotlight(spotlight const&) = default;
	explicit spotlight(spotlight&&) = default;

	spotlight& operator=(spotlight const&) = default;
	spotlight& operator=(spotlight&&) = default;

	inline XMFLOAT3 const& position() const
	{ return position_; }

	inline void position(XMFLOAT3 const &position)
	{ position_ = position; }

private:
	XMFLOAT3 position_ = {};
};
