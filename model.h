#pragma once

#include "texture.h"

class direct3d;

class model
{
public:
	model(direct3d const&, char const *const, char const *const);

	explicit model(model const&) = default;
	explicit model(model&&) = default;
	~model() = default;
	model& operator=(model const&) = default;
	model& operator=(model&&) = default;

	void render();

	inline unsigned index_count() const { return index_count_; }
	inline ComPtr<ID3D11ShaderResourceView> view() const { return texture_.view(); }

private:
	ComPtr<ID3D11Buffer> vertex_buffer_ = nullptr;
	ComPtr<ID3D11Buffer> index_buffer_ = nullptr;
	unsigned index_count_ = 0U;
	texture texture_;

	direct3d const& d3d_;
};