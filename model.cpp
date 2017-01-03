#include "pch.h"
#include "model.h"

#include "direct3d.h"

namespace {
	struct ModelType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
		XMFLOAT3 normal;

		ModelType() = default;
		ModelType(XMFLOAT3 position, XMFLOAT2 texture, XMFLOAT3 normal)
			: position(position), texture(texture), normal(normal) {}
	};
}

std::vector<ModelType> load(char const *const);

model::model(direct3d const& d3d, char const *const modelFilename,
	char const *const textureFilename)
	: d3d_(d3d), texture_(d3d, textureFilename)
{
	HRESULT hResult;

	// Load in the model data
	auto vertices = load(modelFilename);
	index_count_ = static_cast<UINT>(vertices.size());

	auto const vertex_data = D3D11_SUBRESOURCE_DATA
	{
		vertices.data(),
		0U, 0U
	};

	auto const vertex_buffer_desc = D3D11_BUFFER_DESC
	{
		static_cast<UINT>(sizeof(ModelType) * vertices.size()),
		D3D11_USAGE_DEFAULT,
		D3D11_BIND_VERTEX_BUFFER, 0U, 0U,
		0U
	};

	auto const device = d3d_.device();

	hResult = device->CreateBuffer(&vertex_buffer_desc, &vertex_data,
		vertex_buffer_.GetAddressOf());
	if (FAILED(hResult))
	{
		throw "";
	}

	// Create the index buffer.
	std::vector<unsigned long> indices{};
	indices.resize(vertices.size());
	std::iota(indices.begin(), indices.end(), 0UL);

	auto const indexData = D3D11_SUBRESOURCE_DATA
	{
		indices.data(),
		0U, 0U
	};

	auto const indexBufferDesc = D3D11_BUFFER_DESC
	{
		static_cast<UINT>(sizeof(unsigned long) * vertices.size()),
		D3D11_USAGE_DEFAULT,
		D3D11_BIND_INDEX_BUFFER, 0U, 0U,
		0U
	};

	hResult = device->CreateBuffer(&indexBufferDesc, &indexData,
		index_buffer_.GetAddressOf());
	if (FAILED(hResult))
	{
		throw "";
	}
}

void model::render()
{
	auto const context = d3d_.context();

	// Set the vertex buffer to active so it can be rendered.
	auto constexpr stride = static_cast<UINT>(sizeof(ModelType));
	auto constexpr offset = 0U;
	context->IASetVertexBuffers(0U, 1U, vertex_buffer_.GetAddressOf(), &stride,
		&offset);

	// Set the index buffer to active so it can be rendered.
	context->IASetIndexBuffer(index_buffer_.Get(), DXGI_FORMAT_R32_UINT, 0U);

	// Set the type of primitive to triangles.
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

std::vector<ModelType> load(char const *const filename)
{
	// open file
	auto file = std::ifstream{ filename };
	if (file.fail())
	{
		throw "";
	}

	// load file into memory
	auto data = std::stringstream{};
	data << file.rdbuf();
	file.close();

	auto vertex_count = 0U;
	auto texture_count = 0U;
	auto normal_count = 0U;
	auto face_count = 0U;

	// calculate totals
	while (!data.eof()) {
		std::string type{};
		data >> type;
		if ("v" == type) ++vertex_count;
		else if ("vt" == type) ++texture_count;
		else if ("vn" == type) ++normal_count;
		else if ("f" == type) ++face_count;
		data.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
	face_count *= 3U;

	// create vectors to store data
	auto vertices = std::vector<XMFLOAT3>(vertex_count);
	auto textures = std::vector<XMFLOAT2>(texture_count);
	auto normals = std::vector<XMFLOAT3>(normal_count);
	auto faces = std::vector<XMINT3>(face_count);

	// initialise indexes
	auto vertex_index = 0U;
	auto texture_index = 0U;
	auto normal_index = 0U;
	auto face_index = 0U;

	// reset data stream
	data.rdbuf()->pubseekpos(0LL);
	data.clear();

	// load data
	while (!data.eof()) {
		std::string type{};
		data >> type;

		if ("v" == type) {
			data
				>> vertices[vertex_index].x
				>> vertices[vertex_index].y
				>> vertices[vertex_index].z;

			// convert from RHS to LHS
			vertices[vertex_index].z *= -1.f;
			++vertex_index;
		}
		else if ("vt" == type)
		{
			data
				>> textures[texture_index].x
				>> textures[texture_index].y;

			// convert from RHS to LHS
			textures[texture_index].y = 1.f - textures[texture_index].y;
			++texture_index;
		}
		else if ("vn" == type)
		{
			data
				>> normals[normal_index].x
				>> normals[normal_index].y
				>> normals[normal_index].z;

			// convert from RHS to LHS
			normals[normal_index].z *= -1.f;
			++normal_index;
		}
		else if ("f" == type)
		{
			char dummy;

			// read backwards to convert from RHS to LHS
			data
				>> faces[face_index + 2U].x >> dummy
				>> faces[face_index + 2U].y >> dummy
				>> faces[face_index + 2U].z;

			data
				>> faces[face_index + 1U].x >> dummy
				>> faces[face_index + 1U].y >> dummy
				>> faces[face_index + 1U].z;

			data
				>> faces[face_index].x >> dummy
				>> faces[face_index].y >> dummy
				>> faces[face_index].z;

			face_index += 3U;
		}

		data.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}

	// build model
	auto model = std::vector<ModelType>(face_count);
	for (auto const it : faces)
	{
		model.emplace_back
		(
			vertices[it.x - 1U],
			textures[it.y - 1U],
			normals[it.z - 1U]
		);
	}

	return model;
}