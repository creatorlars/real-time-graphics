#pragma once

class light
{
public:
	light() = default;
	~light() = default;

	explicit light(light const&) = default;
	explicit light(light&&) = default;

	light& operator=(light const&) = default;
	light& operator=(light&&) = default;

	inline XMFLOAT3 const& position() const
	{ return position_; }
	inline XMFLOAT3 const& direction() const
	{ return direction_; }
	inline XMFLOAT4 const& ambient() const
	{ return ambient_; }
	inline XMFLOAT4 const& diffuse() const
	{ return diffuse_; }

	inline void position(XMFLOAT3 const &position)
	{ position_ = position; }
	inline void direction(XMFLOAT3 const &direction)
	{ direction_ = direction; }
	inline void ambient(XMFLOAT4 const &ambient)
	{ ambient_ = ambient; }
	inline void diffuse(XMFLOAT4 const &diffuse)
	{ diffuse_ = diffuse; }

private:
	XMFLOAT3 position_ = {};
	XMFLOAT3 direction_ = { -.5f, -.5f, -.5f };
	XMFLOAT4 ambient_ = { .25f, .25f, .25f, 1.f };
	XMFLOAT4 diffuse_ = { .75f, .75f, .75f, 1.f };
};