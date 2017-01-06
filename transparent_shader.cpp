#include "pch.h"
#include "transparent_shader.h"

#include "direct3d.h"
#include "object.h"
#include "camera.h"

transparent_shader::transparent_shader(direct3d const& d3d) : d3d_(d3d)
{
	HRESULT result{};
	auto const device = d3d_.device();

	// Load vertex shader
	auto constexpr vs_path = L"data/transparent_vs.cso";
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
	auto constexpr ps_path = L"data/transparent_ps.cso";
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
	auto constexpr buffer_desc = D3D11_BUFFER_DESC
	{
		sizeof(MatrixBufferType),
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

	auto constexpr transparent_buffer_desc = D3D11_BUFFER_DESC
	{
		sizeof(TransparentBufferType),
		D3D11_USAGE_DYNAMIC,
		D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE, 0U, 0U
	};

	// Create the constant buffer pointer so we can access the pixel shader
	// constant buffer from within this class.
	result = device->CreateBuffer(&transparent_buffer_desc, nullptr,
		transparent_buffer_.GetAddressOf());
	if (FAILED(result))
	{
		throw "";
	}
}