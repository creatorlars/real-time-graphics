#include "pch.h"
#include "particle.h"

#include "direct3d.h"

namespace
{
	struct VertexType
	{
		XMFLOAT3 position_;
		XMFLOAT2 texture_;

		constexpr VertexType(XMFLOAT3 const &position, XMFLOAT2 const &texture)
			: position_(position), texture_(texture)
		{}

		constexpr VertexType(XMFLOAT3 &&position, XMFLOAT2 &&texture)
			: position_(std::move(position)), texture_(std::move(texture))
		{}

		VertexType() = delete;
		~VertexType() = default;

		constexpr explicit VertexType(VertexType const&) = default;
		constexpr explicit VertexType(VertexType&&) = default;

		VertexType& operator=(VertexType const&) = default;
		VertexType& operator=(VertexType&&) = default;
	};
}

particle::particle(direct3d const& d3d, char const *const filename,
	unsigned life)
	: d3d_(d3d), texture_(d3d, filename), life_(life)
{
	HRESULT result{};

	auto constexpr vertices = std::array<VertexType, index_count_>
	{ {
		{ { -1.f, -1.f, 0.f }, { 0.f, 1.f } },	// bottom left
		{ { -1.f, 1.f, 0.f }, { 0.f, 0.f } },	// top left
		{ { 1.f, -1.f, 0.0f }, { -1.f, 1.f } },	// bottom right
		{ { 1.f, 1.f, 0.0f }, { -1.f, 0.f } },	// top right
		{ { 1.f, -1.f, 0.0f }, { -1.f, 1.f } },	// bottom right
		{ { -1.f, 1.f, 0.0f }, { 0.f, 1.f } }	// top left
		
	} };

	auto constexpr vertex_buffer_desc = D3D11_BUFFER_DESC
	{
		static_cast<UINT>(sizeof(VertexType) * vertices.size()),
		D3D11_USAGE_DEFAULT,
		D3D11_BIND_VERTEX_BUFFER, 0U, 0U,
		0U
	};

	auto const vertex_data = D3D11_SUBRESOURCE_DATA
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

	auto const index_data = D3D11_SUBRESOURCE_DATA
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

	auto const xmmatrix = XMMatrixIdentity();
	XMStoreFloat4x4(&matrix_, xmmatrix);
}

void particle::render()
{
	if (life_ == 0U)
	{
		return;
	}

	auto xmmatrix = XMMatrixIdentity();
	xmmatrix *= XMMatrixScaling(scale_.x, scale_.y, scale_.z);

	xmmatrix *= XMMatrixTranslation(position_.x, position_.y, position_.z);

	XMStoreFloat4x4(&matrix_, xmmatrix);

	auto const context = d3d_.context();

	// Set the vertex buffer to active so it can be rendered.
	auto constexpr stride = static_cast<UINT>(sizeof(VertexType));
	auto constexpr offset = 0U;
	context->IASetVertexBuffers(0U, 1U, vertex_buffer_.GetAddressOf(), &stride,
		&offset);

	// Set the index buffer to active so it can be rendered.
	context->IASetIndexBuffer(index_buffer_.Get(), DXGI_FORMAT_R32_UINT, 0U);

	// Set the type of primitive to triangles.
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void particle::frame()
{
	--life_;
	if (life_ == 0U)
	{
		return;
	}

	position_.x += direction_.x;
	position_.y += direction_.y;
	position_.z += direction_.z;
}