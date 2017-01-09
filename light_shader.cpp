#include "pch.h"
#include "light_shader.h"

#include "direct3d.h"
#include "object.h"
#include "camera.h"

#include "ambient.h"
#include "spotlight.h"

namespace
{
	auto constexpr MAXLIGHTS = 10U;

	struct MATRICES
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

	struct LightPositionBufferType
	{
		std::array<XMVECTOR, MAXLIGHTS> position;
		int count;
	};

	struct LightBufferType
	{
		std::array<XMVECTOR, MAXLIGHTS> spotlight_colours;
		XMVECTOR ambient_min;
		XMVECTOR ambient_max;
		XMVECTOR ambient_direction;
		float exposure;
		float gamma;
		int spotlight_count;
	};
}

light_shader::light_shader(direct3d const& d3d) : exposure_(1.f), gamma_(1.f), d3d_(d3d)
{
	HRESULT result{};
	auto const device = d3d_.device();

	// Load vertex shader
	auto constexpr vs_path = L"data/light_vs.cso";
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
	auto constexpr ps_path = L"data/light_ps.cso";
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
	const auto polygon_layout = std::array<D3D11_INPUT_ELEMENT_DESC, 3U>
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
		},
		{
			"NORMAL",
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

	// Create a texture sampler state description.
	auto constexpr sampler_desc = D3D11_SAMPLER_DESC
	{
		D3D11_FILTER_MIN_MAG_MIP_LINEAR,
		D3D11_TEXTURE_ADDRESS_WRAP, D3D11_TEXTURE_ADDRESS_WRAP, D3D11_TEXTURE_ADDRESS_WRAP,
		0.f, 1U, D3D11_COMPARISON_ALWAYS,
		0.f, 0.f, 0.f, 0.f,
		0.f, D3D11_FLOAT32_MAX
	};

	// Create the texture sampler state.
	result = device->CreateSamplerState(&sampler_desc,
		sampler_state_.GetAddressOf());
	if (FAILED(result))
	{
		throw "";
	}

	// Setup the description of the dynamic matrix constant buffer that is in
	// the vertex shader.
	auto constexpr buffer_desc = D3D11_BUFFER_DESC
	{
		sizeof(MATRICES),
		D3D11_USAGE_DYNAMIC,
		D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0U,
		0U
	};

	// Create the constant buffer pointer so we can access the vertex shader
	// constant buffer from within this class.
	result = device->CreateBuffer(&buffer_desc, nullptr,
		matrix_buffer_.GetAddressOf());
	if (FAILED(result))
	{
		throw "";
	}

	auto constexpr light_buffer_desc = D3D11_BUFFER_DESC
	{
		sizeof(LightBufferType),
		D3D11_USAGE_DYNAMIC,
		D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0U, 0U
	};

	// Create the constant buffer pointer so we can access the pixel shader
	// constant buffer from within this class.
	result = device->CreateBuffer(&light_buffer_desc, nullptr,
		light_buffer_.GetAddressOf());
	if (FAILED(result))
	{
		throw "";
	}

	auto constexpr light_position_buffer_desc = D3D11_BUFFER_DESC
	{
		sizeof(LightBufferType),
		D3D11_USAGE_DYNAMIC,
		D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0U, 0U
	};

	// Create the constant buffer pointer so we can access the pixel shader
	// constant buffer from within this class.
	result = device->CreateBuffer(&light_position_buffer_desc, nullptr,
		light_position_buffer_.GetAddressOf());
	if (FAILED(result))
	{
		throw "";
	}
}

void light_shader::render(XMMATRIX const &world, XMMATRIX const &view,
	ComPtr<ID3D11ShaderResourceView> const &texture, unsigned const index_count)
	const
{
	HRESULT result{};
	auto const context = d3d_.context();

	//object->render();

	// Lock the constant buffer so it can be written to.
	auto mapped_resource = D3D11_MAPPED_SUBRESOURCE{};
	result = context->Map(matrix_buffer_.Get(), 0U, D3D11_MAP_WRITE_DISCARD, 0U,
		&mapped_resource);
	if (FAILED(result))
	{
		throw "";
	}

	// Get a pointer to the data in the constant buffer.
	auto const matrix_buffer = static_cast<MATRICES*>(mapped_resource.pData);

	// Copy the matrices into the constant buffer.
	matrix_buffer->projection = XMMatrixTranspose(d3d_.projection_matrix());
	matrix_buffer->world = XMMatrixTranspose(world);
	matrix_buffer->view = XMMatrixTranspose(view);

	// Unlock the constant buffer.
	context->Unmap(matrix_buffer_.Get(), 0U);

	// finally set the constant buffer in the vertex shader
	context->VSSetConstantBuffers(0U, 1U, matrix_buffer_.GetAddressOf());






	// Set shader texture resource in the pixel shader.
	context->PSSetShaderResources(0U, 1U, texture.GetAddressOf());

	// Lock the transparent constant buffer so it can be written to.
	result = context->Map(light_buffer_.Get(), 0U, D3D11_MAP_WRITE_DISCARD, 0U,
		&mapped_resource);
	if (FAILED(result))
	{
		throw "";
	}

	auto const light_buffer = static_cast<LightBufferType*>(mapped_resource.pData);

	// store ambient information
	light_buffer->ambient_min = XMLoadFloat3(&ambient_->min());
	light_buffer->ambient_max = XMLoadFloat3(&ambient_->max());
	light_buffer->ambient_direction = XMLoadFloat3(&ambient_->direction());

	light_buffer->exposure = exposure_;
	light_buffer->gamma = gamma_;

	// store spotlight information
	light_buffer->spotlight_count = static_cast<int>(spotlights_.size());
	for (auto i = size_t{}; i < spotlights_.size(); ++i)
	{
		// Get vectors
		auto const spotlight_colour = XMLoadFloat3(&spotlights_[i]->colour());

		// Copy the matrices into the constant buffer
		light_buffer->spotlight_colours[i] = spotlight_colour;
	}

	// Unlock the buffer.
	context->Unmap(light_buffer_.Get(), 0U);

	// Now set the texture translation constant buffer in the pixel shader with
	// the updated values.
	context->PSSetConstantBuffers(0U, 1U, light_buffer_.GetAddressOf());





	result = context->Map(light_position_buffer_.Get(), 0U, D3D11_MAP_WRITE_DISCARD, 0U,
		&mapped_resource);
	if (FAILED(result))
	{
		throw "";
	}

	// Get a pointer to the data in the constant buffer.
	auto const light_position_buffer = static_cast<LightPositionBufferType*>(mapped_resource.pData);
	light_position_buffer->count = static_cast<int>(spotlights_.size());
	for (auto i = size_t{}; i < spotlights_.size(); ++i)
	{
		// store position
		auto const position = XMLoadFloat3(&spotlights_[i]->position());
		light_position_buffer->position[i] = position;
	}

	// Unlock the constant buffer.
	context->Unmap(light_position_buffer_.Get(), 0U);

	// finally set the constant buffer in the vertex shader
	context->VSSetConstantBuffers(1U, 1U, light_position_buffer_.GetAddressOf());





	// Set the vertex input layout.
	context->IASetInputLayout(layout_.Get());

	// Set the vertex and pixel shaders used to render this triangle.
	context->VSSetShader(vertex_shader_.Get(), nullptr, 0U);
	context->PSSetShader(pixel_shader_.Get(), nullptr, 0U);

	// Set the sampler state in the pixel shader.
	context->PSSetSamplers(0U, 1U, sampler_state_.GetAddressOf());

	// Render the triangle.
	context->DrawIndexed(index_count, 0U, 0);
}