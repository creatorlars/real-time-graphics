#pragma once

class ambient
{
public:
	ambient(XMFLOAT3 const&, XMFLOAT3 const&, XMFLOAT3 const&);

	ambient() = delete;
	~ambient() = default;

	explicit ambient(ambient const&) = default;
	explicit ambient(ambient&&) = default;

	ambient& operator=(ambient const&) = default;
	ambient& operator=(ambient&&) = default;

	inline XMFLOAT3 const& direction() const
	{ return direction_; }
	inline XMFLOAT3 const& min() const
	{ return min_; }
	inline XMFLOAT3 const& max() const
	{ return max_; }
	
	inline void direction(XMFLOAT3 const &direction)
	{ direction_ = direction; }
	inline void min(XMFLOAT3 const &min)
	{ min_ = min; }
	inline void max(XMFLOAT3 const &max)
	{ max_ = max; }

private:
	XMFLOAT3 direction_ = {};
	XMFLOAT3 min_ = {};
	XMFLOAT3 max_ = {};
};
