#pragma once

#define TEST_MATRIX_VEC 1
#if TEST_MATRIX_VEC
class matrix
{
public:
	matrix() = default;
	~matrix() = default;

	explicit matrix(matrix const&) = default;
	explicit matrix(matrix&&) = default;

	matrix& operator=(matrix const&) = default;
	matrix& operator=(matrix&&) = default;

	inline XMFLOAT3 const& rotation() const
	{ return rotation_; }
	inline XMFLOAT3 const& position() const
	{ return position_; }	
	inline XMFLOAT3 const& scale() const
	{ return scale_; }

	inline void rotation(XMFLOAT3 const& rotation)
	{ rotation_ = rotation; }
	inline void position(XMFLOAT3 const& position)
	{ position_ = position; }
	inline void scale(XMFLOAT3 const& scale)
	{ scale_ = scale; }

	void rotate(XMFLOAT3 const&);
	void translate(XMFLOAT3 const&);
	void transform(XMFLOAT3 const&);

private:
	XMFLOAT3 rotation_ = {};
	XMFLOAT3 position_ = {};
	XMFLOAT3 scale_ = { 1.f, 1.f, 1.f };
};
#else
class matrix
{
public:
	matrix() = default;
	~matrix() = default;

	explicit matrix(matrix const&) = default;
	explicit matrix(matrix&&) = default;

	matrix& operator=(matrix const&) = default;
	matrix& operator=(matrix&&) = default;

	inline XMVECTOR const& rotation() const
	{ return rotation_; }
	inline XMVECTOR const& position() const
	{ return position_; }	
	inline XMVECTOR const& scale() const
	{ return scale_; }

	inline void rotation(XMVECTOR const &rotation)
	{ rotation_ = rotation; }
	inline void position(XMVECTOR const &position)
	{ position_ = position; }
	inline void scale(XMVECTOR const &scale)
	{ scale_ = scale; }

	inline void rotate(XMVECTOR const &delta)
	{ rotation_ = XMVectorAdd(rotation_, delta); }
	inline void translate(XMVECTOR const &delta)
	{ position_ = XMVectorAdd(position_, delta); }
	inline void transform(XMVECTOR const &delta)
	{ scale_ = XMVectorMultiply(scale_, delta); }

private:
	XMVECTOR rotation_ = {};
	XMVECTOR position_ = {};
	XMVECTOR scale_ = { 1.f, 1.f, 1.f };
};
#endif