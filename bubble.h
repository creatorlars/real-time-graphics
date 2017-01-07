#pragma once

#include "quad.h"
#include "texture.h"

class direct3d;
class camera;

class bubble
{
public:
	bubble(direct3d const&, texture const&, XMFLOAT3 const&,
		std::shared_ptr<camera> const&, unsigned const);

	void frame();
	void render();

	inline XMFLOAT4X4 const& matrix()
	{ return matrix_; }
	inline unsigned const index_count() const
	{ return quad_.index_count(); }
	inline ComPtr<ID3D11ShaderResourceView> const& view() const
	{ return texture_.view(); }
	inline bool alive() const
	{ return life_ != 0U; }

	inline void matrix(XMFLOAT4X4 const &matrix)
	{ matrix_ = matrix; }

	void move(XMFLOAT3 const&);

private:
	XMFLOAT4X4 matrix_ = {};
	XMFLOAT3 position_ = {};
	XMFLOAT3 scale_ = { .01f, .01f, .01f };

	texture const &texture_;
	unsigned life_ = 0U;

	quad quad_;

	std::shared_ptr<camera> camera_ = nullptr;
};