#pragma once

#include "direct3d.h"
#include "camera.h"

class object;

class transparent_shader
{
public:
	explicit transparent_shader(direct3d const&);

	transparent_shader() = delete;
	~transparent_shader() = default;

	explicit transparent_shader(transparent_shader const&) = default;
	explicit transparent_shader(transparent_shader&&) = default;

	transparent_shader& operator=(transparent_shader const&) = default;
	transparent_shader& operator=(transparent_shader&&) = default;

	template <typename T>
	void render(T const &object, std::shared_ptr<camera> const &camera,
		float const blend) const
	{
		render(object->render(), camera->render(), object->view(),
			object->index_count(), blend);
	}

private:
	void render(XMMATRIX const&, XMMATRIX const&,
		ComPtr<ID3D11ShaderResourceView> const&, unsigned const, float const)
		const;

	ComPtr<ID3D11VertexShader> vertex_shader_ = nullptr;
	ComPtr<ID3D11PixelShader> pixel_shader_ = nullptr;
	ComPtr<ID3D11InputLayout> layout_ = nullptr;
	ComPtr<ID3D11Buffer> matrix_buffer_ = nullptr;
	ComPtr<ID3D11SamplerState> sampler_state_ = nullptr;
	ComPtr<ID3D11Buffer> transparent_buffer_ = nullptr;

	direct3d const& d3d_;
};