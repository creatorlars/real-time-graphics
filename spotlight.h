#pragma once

class spotlight
{
public:
	spotlight(XMFLOAT3 const&, XMFLOAT3 const&);

	spotlight() = delete;
	~spotlight() = default;

	explicit spotlight(spotlight const&) = default;
	explicit spotlight(spotlight&&) = default;

	spotlight& operator=(spotlight const&) = default;
	spotlight& operator=(spotlight&&) = default;

	inline XMFLOAT3 const& position() const
	{ return position_; }
	inline XMFLOAT3 const& colour() const
	{ return colour_; }

	inline void position(XMFLOAT3 const &position)
	{ position_ = position; }
	inline void colour(XMFLOAT3 const &colour)
	{ colour_ = colour; }

private:
	XMFLOAT3 position_ = {};
	XMFLOAT3 colour_ = {};
};
