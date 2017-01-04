#include "pch.h"
#include "texture_shader.h"

#include "direct3d.h"
#include "object.h"
#include "camera.h"

namespace {
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};
}

texture_shader::texture_shader(direct3d const& d3d) : d3d_(d3d)
{
	HRESULT result{};
	auto const device = d3d_.device();

	// Load vertex shader
	auto constexpr vs_path = L"data/VertexShader.cso";
	auto vs_stream = std::stringstream{};
	vs_stream << std::ifstream{ vs_path, std::ios::binary }.rdbuf();
	auto const vs_data = vs_stream.str();

	result = device->CreateVertexShader(vs_data.data(), vs_data.size(), nullptr,
		vertex_shader_.GetAddressOf());
	if (FAILED(result))
	{
		throw "";
	}

	// Load pixel shader
	auto constexpr ps_path = L"data/PixelShader.cso";
	auto ps_stream = std::stringstream{};
	ps_stream << std::ifstream{ ps_path, std::ios::binary }.rdbuf();
	auto const ps_data = ps_stream.str();

	result = device->CreatePixelShader(ps_data.data(), ps_data.size(), nullptr,
		pixel_shader_.GetAddressOf());
	if (FAILED(result))
	{
		throw "";
	}

	// Create the vertex input layout description. This setup needs to match the
	// VertexType stucture in the ModelClass and in the shader.
	const auto polygon_layout = std::array<D3D11_INPUT_ELEMENT_DESC, 2U>
	{{
		{
			"POSITION",
			0U,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0U,
			0U,
			D3D11_INPUT_PER_VERTEX_DATA,
			0U
		},
		{
			"TEXCOORD",
			0U,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0U,
			D3D11_APPEND_ALIGNED_ELEMENT,
			D3D11_INPUT_PER_VERTEX_DATA,
			0U
		}
	}};

	result = device->CreateInputLayout(polygon_layout.data(),
		static_cast<UINT>(polygon_layout.size()), vs_data.data(),
		static_cast<UINT>(vs_data.size()), layout_.GetAddressOf());
	if (FAILED(result))
	{
		throw "";
	}

	// Setup the description of the dynamic matrix constant buffer that is in
	// the vertex shader.
	auto constexpr buffer_description = D3D11_BUFFER_DESC
	{
		sizeof(MatrixBufferType),
		D3D11_USAGE_DYNAMIC,
		D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0U,
		0U
	};

	// Create the constant buffer pointer so we can access the vertex shader
	// constant buffer from within this class.
	result = device->CreateBuffer(&buffer_description, nullptr,
		matrix_buffer_.GetAddressOf());
	if (FAILED(result))
	{
		throw "";
	}

	// Create a texture sampler state description.
	auto constexpr sampler_description = D3D11_SAMPLER_DESC
	{
		D3D11_FILTER_MIN_MAG_MIP_LINEAR,
		D3D11_TEXTURE_ADDRESS_WRAP, D3D11_TEXTURE_ADDRESS_WRAP, D3D11_TEXTURE_ADDRESS_WRAP,
		0.f, 1U, D3D11_COMPARISON_ALWAYS,
		0.f, 0.f, 0.f, 0.f,
		0.f, D3D11_FLOAT32_MAX
	};

	// Create the texture sampler state.
	result = device->CreateSamplerState(&sampler_description,
		sample_state_.GetAddressOf());
	if (FAILED(result))
	{
		throw "";
	}
}

void texture_shader::render(std::shared_ptr<object> const &object,
	std::shared_ptr<camera> const &camera) const
{
	auto const context = d3d_.context();

	//auto object_matrix = XMLoadFloat4x4(&d3d_.world_matrix());
	//auto world_matrix = XMLoadFloat4x4(&object->matrix());
	//object_matrix += world_matrix;
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
	auto const matrix_buffer = static_cast<MatrixBufferType*>(mapped_resource.pData);

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
	auto const texture = object->model()->view();
	context->PSSetShaderResources(0U, 1U, texture.GetAddressOf());

	// Set the vertex input layout.
	context->IASetInputLayout(layout_.Get());

	// Set the vertex and pixel shaders that will be used to render this
	// triangle.
	context->VSSetShader(vertex_shader_.Get(), nullptr, 0U);
	context->PSSetShader(pixel_shader_.Get(), nullptr, 0U);

	// Set the sampler state in the pixel shader.
	context->PSSetSamplers(0U, 1U, sample_state_.GetAddressOf());

	// Render the triangle.
	context->DrawIndexed(object->model()->index_count(), 0U, 0);
}