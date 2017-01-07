#pragma once

#include "direct3d.h"
#include "camera.h"

class object;

class texture_shader
{
public:
	explicit texture_shader(direct3d const&);

	texture_shader() = delete;
	~texture_shader() = default;

	explicit texture_shader(texture_shader const&) = default;
	explicit texture_shader(texture_shader&&) = default;

	texture_shader& operator=(texture_shader const&) = default;
	texture_shader& operator=(texture_shader&&) = default;

	template <typename T>
	void render(T const &object, std::shared_ptr<camera> const &camera) const
	{
		render(object->render(), camera->render(), object->view(),
			object->index_count());
	}

private:
	void render(XMMATRIX const&, XMMATRIX const&,
		ComPtr<ID3D11ShaderResourceView> const&, unsigned const) const;

	ComPtr<ID3D11VertexShader> vertex_shader_ = nullptr;
	ComPtr<ID3D11PixelShader> pixel_shader_ = nullptr;
	ComPtr<ID3D11InputLayout> layout_ = nullptr;
	ComPtr<ID3D11Buffer> matrix_buffer_ = nullptr;
	ComPtr<ID3D11SamplerState> sampler_state_ = nullptr;

	direct3d const& d3d_;
};