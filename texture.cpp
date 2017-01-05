#include "pch.h"
#include "texture.h"

#include "direct3d.h"

struct ImageData
{
	std::vector<unsigned char> data;
	unsigned width;
	unsigned height;
};

ImageData load(char const *const);

texture::texture(direct3d const& d3d, char const *const filename)
{
	HRESULT result{};

	auto const image = load(filename);

	// Create the empty texture.
	auto const texture_desc = D3D11_TEXTURE2D_DESC
	{
		image.width,
		image.height,
		0U,
		1U,
		DXGI_FORMAT_R8G8B8A8_UNORM,
		{ 1U, 0U },
		D3D11_USAGE_DEFAULT,
		D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET,
		0U,
		D3D11_RESOURCE_MISC_GENERATE_MIPS
	};

	auto const device = d3d.device();
	result = device->CreateTexture2D(&texture_desc, nullptr,
		texture_.GetAddressOf());
	if (FAILED(result))
	{
		throw "";
	}

	// Copy the image data into the texture.
	auto const context = d3d.context();
	auto const row_pitch = (image.width * 4U) * sizeof(unsigned char);
	context->UpdateSubresource(texture_.Get(), 0U, nullptr, image.data.data(),
		static_cast<UINT>(row_pitch), 0U);

	// Create the shader resource view for the texture.
	auto const shader_resource_view_desc = D3D11_SHADER_RESOURCE_VIEW_DESC
	{
		texture_desc.Format,
		D3D11_SRV_DIMENSION_TEXTURE2D,
		{ 0U, UINT_MAX }
	};

	result = device->CreateShaderResourceView(texture_.Get(),
		&shader_resource_view_desc, view_.GetAddressOf());
	if (FAILED(result))
	{
		throw "";
	}

	// Generate mipmaps for this texture.
	context->GenerateMips(view_.Get());
}

ImageData load(char const *const filename)
{
	struct targa_header
	{
		unsigned char unused1[12];
		unsigned short width;
		unsigned short height;
		unsigned char bpp;
		unsigned char unused2;
	};

	auto file = std::ifstream{ filename, std::ios::binary };

	// read image header
	auto header = targa_header{};
	file.read(reinterpret_cast<char *>(&header), sizeof(targa_header));
	auto const size = static_cast<size_t>(header.width * header.height
		* (header.bpp / 8U));

	// read image data
	auto stream = std::stringstream{};
	stream << file.rdbuf();
	auto data = stream.str();
	data.resize(size);

	// normalise image data (BGRA -> RGBA)
	auto out = std::vector<unsigned char>(size);
	for (auto i = size_t{}, j = size; i < size; i += 4U) {
		j -= 4U;
		out[i + 0U] = data[j + 2U];  // Red.
		out[i + 1U] = data[j + 1U];  // Green.
		out[i + 2U] = data[j + 0U];  // Blue
		out[i + 3U] = data[j + 3U];  // Alpha
	}

	return{ out, header.width, header.height };
}