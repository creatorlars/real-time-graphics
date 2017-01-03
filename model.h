#pragma once

#include "texture.h"

class direct3d;

class model
{
public:
	model(direct3d const&, char const *const, char const *const);

	void render();

	inline unsigned index_count() const { return index_count_; }
	inline ComPtr<ID3D11ShaderResourceView> view() const { return texture_.view(); }

private:
	ComPtr<ID3D11Buffer> vertex_buffer_ = nullptr;
	ComPtr<ID3D11Buffer> index_buffer_ = nullptr;
	unsigned index_count_;
	texture texture_;

	direct3d const& d3d_;
};