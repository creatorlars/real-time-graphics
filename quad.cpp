#include "pch.h"
#include "quad.h"

#include "direct3d.h"

namespace
{
	struct VERTEX
	{
		XMFLOAT3 position_;
		XMFLOAT2 texture_;
		XMFLOAT4 color_;

		constexpr VERTEX(XMFLOAT3 const &position, XMFLOAT2 const &texture,
			XMFLOAT4 const &color)
			: position_(position), texture_(texture), color_(color)
		{}

		constexpr VERTEX(XMFLOAT3 &&position, XMFLOAT2 &&texture,
			XMFLOAT4 &&color)
			: position_(std::move(position)), texture_(std::move(texture)),
			color_(std::move(color))
		{}

		VERTEX() = delete;
		~VERTEX() = default;

		constexpr explicit VERTEX(VERTEX const&) = default;
		constexpr explicit VERTEX(VERTEX&&) = default;

		VERTEX& operator=(VERTEX const&) = default;
		VERTEX& operator=(VERTEX&&) = default;
	};
}

quad::quad(direct3d const& d3d) : d3d_(d3d)
{
	HRESULT result{};

	auto constexpr vertices = std::array<VERTEX, index_count_>
	{ {
		{ { -1.f, -1.f, 0.f }, { 1.f, 1.f }, {} },	// bottom left
		{ { -1.f, 1.f, 0.f }, { 1.f, 0.f }, {} },	// top left
		{ { 1.f, -1.f, 0.0f }, { 0.f, 1.f }, {} },	// bottom right
		{ { 1.f, 1.f, 0.0f }, { 0.f, 0.f }, {} },	// top right
		{ { 1.f, -1.f, 0.0f }, { 0.f, 1.f }, {} },	// bottom right
		{ { -1.f, 1.f, 0.0f }, { 1.f, 0.f }, {} }	// top left
	} };

	auto constexpr vertex_buffer_desc = D3D11_BUFFER_DESC
	{
		static_cast<UINT>(sizeof(VERTEX) * vertices.size()),
		D3D11_USAGE_DEFAULT,
		D3D11_BIND_VERTEX_BUFFER, 0U, 0U,
		0U
	};

	static auto const vertex_data = D3D11_SUBRESOURCE_DATA
	{
		vertices.data(),
		0U, 0U
	};

	// Now create the vertex buffer.
	auto const device = d3d.device();
	result = device->CreateBuffer(&vertex_buffer_desc, &vertex_data,
		vertex_buffer_.GetAddressOf());
	if (FAILED(result))
	{
		throw "";
	}

	// Create the index buffer.
	auto constexpr indices = std::array<unsigned long, index_count_>
	{ { 0UL, 1UL, 2UL, 3UL, 4UL, 5UL } };

	auto constexpr index_buffer_desc = D3D11_BUFFER_DESC
	{
		static_cast<UINT>(sizeof(unsigned long) * indices.size()),
		D3D11_USAGE_DEFAULT,
		D3D11_BIND_INDEX_BUFFER, 0U, 0U,
		0U
	};

	static auto const index_data = D3D11_SUBRESOURCE_DATA
	{
		indices.data(),
		0U, 0U
	};

	result = device->CreateBuffer(&index_buffer_desc, &index_data,
		index_buffer_.GetAddressOf());
	if (FAILED(result))
	{
		throw "";
	}
}

void quad::render() const
{
	auto const context = d3d_.context();

	// Set the vertex buffer to active so it can be rendered.
	auto constexpr stride = static_cast<UINT>(sizeof(VERTEX));
	auto constexpr offset = 0U;
	context->IASetVertexBuffers(0U, 1U, vertex_buffer_.GetAddressOf(), &stride,
		&offset);

	// Set the index buffer to active so it can be rendered.
	context->IASetIndexBuffer(index_buffer_.Get(), DXGI_FORMAT_R32_UINT, 0U);

	// Set the type of primitive to triangles.
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}