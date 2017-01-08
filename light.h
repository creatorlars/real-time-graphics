#pragma once

class light
{
public:
	light(XMFLOAT4 const&, XMFLOAT4 const&);

	light() = delete;
	virtual ~light() = default;

	explicit light(light const&) = default;
	explicit light(light&&) = default;

	light& operator=(light const&) = default;
	light& operator=(light&&) = default;

	inline XMFLOAT4 const& max() const
	{ return max_; }
	inline XMFLOAT4 const& min() const
	{ return min_; }

	inline void max(XMFLOAT4 const &ambient)
	{ max_ = ambient; }
	inline void min(XMFLOAT4 const &diffuse)
	{ min_ = diffuse; }

private:
	XMFLOAT4 min_ = {};
	XMFLOAT4 max_ = {};
};