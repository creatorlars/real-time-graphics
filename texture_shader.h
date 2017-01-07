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
	void render(std::shared_ptr<T> const &object,
		std::shared_ptr<camera> const &camera) const
	{
		auto const context = d3d_.context();

		object->render();

		// Lock the constant buffer so it can be written to.
		auto mapped_resource = D3D11_MAPPED_SUBRESOURCE{};
		auto const result = context->Map(matrix_buffer_.Get(), 0U,
			D3D11_MAP_WRITE_DISCARD, 0U, &mapped_resource);
		if (FAILED(result))
		{
			throw "";
		}

		// Get a pointer to the data in the constant buffer.
		auto const matrix_buffer = static_cast<MATRICES*>(mapped_resource.pData);

		// Get matrices.
		auto const projection_matrix = XMLoadFloat4x4(&d3d_.projection_matrix());
		auto const object_matrix = XMLoadFloat4x4(&object->matrix());
		auto const camera_matrix = XMLoadFloat4x4(&camera->matrix());

		// Copy the matrices into the constant buffer.
		matrix_buffer->projection = XMMatrixTranspose(projection_matrix);
		matrix_buffer->world = XMMatrixTranspose(object_matrix);
		matrix_buffer->view = XMMatrixTranspose(camera_matrix);

		// Unlock the constant buffer.
		context->Unmap(matrix_buffer_.Get(), 0U);

		// Finanly set the constant buffer in the vertex shader with the updated
		// values.
		context->VSSetConstantBuffers(0U, 1U, matrix_buffer_.GetAddressOf());

		// Set shader texture resource in the pixel shader.
		auto const texture = object->view();
		context->PSSetShaderResources(0U, 1U, texture.GetAddressOf());

		// Set the vertex input layout.
		context->IASetInputLayout(layout_.Get());

		// Set the vertex and pixel shaders that will be used to render this
		// triangle.
		context->VSSetShader(vertex_shader_.Get(), nullptr, 0U);
		context->PSSetShader(pixel_shader_.Get(), nullptr, 0U);

		// Set the sampler state in the pixel shader.
		context->PSSetSamplers(0U, 1U, sampler_state_.GetAddressOf());

		// Render the triangle.
		context->DrawIndexed(object->index_count(), 0U, 0);
	}

private:
	struct MATRICES
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

	ComPtr<ID3D11VertexShader> vertex_shader_ = nullptr;
	ComPtr<ID3D11PixelShader> pixel_shader_ = nullptr;
	ComPtr<ID3D11InputLayout> layout_ = nullptr;
	ComPtr<ID3D11Buffer> matrix_buffer_ = nullptr;
	ComPtr<ID3D11SamplerState> sampler_state_ = nullptr;

	direct3d const& d3d_;
};