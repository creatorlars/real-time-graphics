////////////////////////////////////////////////////////////////////////////////
// Filename: textureclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "pch.h"
#include "texture.h"

#include "direct3d.h"

texture::texture(direct3d const& d3d, char const *const filename)
{
	HRESULT hResult;

	auto const image = load(filename);

	// Create the empty texture.
	auto const textureDesc = D3D11_TEXTURE2D_DESC
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
	auto const context = d3d.context();

	hResult = device->CreateTexture2D(&textureDesc, NULL, texture_.GetAddressOf());
	if (FAILED(hResult))
	{
		throw "";
	}

	// Copy the image data into the texture.
	auto rowPitch = (image.width * 4) * sizeof(unsigned char);
	context->UpdateSubresource(texture_.Get(), 0U, NULL, image.data.data(), rowPitch, 0U);

	// Create the shader resource view for the texture.
	auto const srvDesc = D3D11_SHADER_RESOURCE_VIEW_DESC
	{
		textureDesc.Format,
		D3D11_SRV_DIMENSION_TEXTURE2D,
		{ 0U, UINT_MAX }
	};

	hResult = device->CreateShaderResourceView(texture_.Get(), &srvDesc, view_.GetAddressOf());
	if (FAILED(hResult))
	{
		throw "";
	}

	// Generate mipmaps for this texture.
	context->GenerateMips(view_.Get());
}

texture::ImageData texture::load(char const *const filename)
{
	struct TargaHeader
	{
		unsigned char data1[12];
		unsigned short width;
		unsigned short height;
		unsigned char bpp;
		unsigned char data2;
	};

	auto file = std::ifstream{ filename, std::ios::binary };

	// read image header
	auto header = TargaHeader{};
	file.read(reinterpret_cast<char *>(&header), sizeof(TargaHeader));
	auto const imageSize = header.width * header.height * (header.bpp / 8U);

	// read image data
	auto stream = std::stringstream{ std::ios::binary | std::ios::in | std::ios::out };
	stream << file.rdbuf();
	auto data = stream.str();
	data.resize(imageSize);

	// normalise image data (BGRA -> RGBA)
	auto out = std::vector<unsigned char>(imageSize);
	for (auto i = 0U, j = imageSize; i < imageSize; i += 4U) {
		j -= 4U;
		out[i + 0U] = data[j + 2U];  // Red.
		out[i + 1U] = data[j + 1U];  // Green.
		out[i + 2U] = data[j + 0U];  // Blue
		out[i + 3U] = data[j + 3U];  // Alpha
	}

	return{ out, header.width, header.height };
}