#pragma once

#include "texture.h"

class direct3d;

class particle
{
public:
	particle(direct3d const&, char const *const, unsigned);

	particle() = delete;
	~particle() = default;

	explicit particle(particle const&) = default;
	explicit particle(particle&&) = default;

	particle& operator=(particle const&) = default;
	particle& operator=(particle&&) = default;

	void frame();
	void render();

	inline XMFLOAT4X4 const& matrix() const
	{ return matrix_; }

	inline unsigned index_count() const
	{ return index_count_; }
	inline ComPtr<ID3D11ShaderResourceView> view() const
	{ return texture_.view(); }
	inline void matrix(XMFLOAT4X4 const &matrix)
	{ matrix_ = matrix;	}
	inline bool alive() const
	{ return life_ != 0U; }

private:
	ComPtr<ID3D11Buffer> vertex_buffer_ = nullptr;
	ComPtr<ID3D11Buffer> index_buffer_ = nullptr;

	direct3d const &d3d_;
	constexpr static unsigned index_count_ = 6U;
	texture texture_;

	XMFLOAT4X4 matrix_ = {};
	XMFLOAT3 position_ = { 1.f, 1.f, 1.f};
	XMFLOAT3 direction_ = {};
	XMFLOAT3 scale_ = { .1f, .1f, .1f };

	unsigned life_ = {};
};