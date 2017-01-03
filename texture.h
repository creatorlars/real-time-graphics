#pragma once

class direct3d;

class texture
{
public:
	texture(direct3d const&, char const *const);

	inline ComPtr<ID3D11ShaderResourceView> const& view() const { return view_; }

private:
	struct ImageData
	{
		std::vector<unsigned char> data;
		unsigned width;
		unsigned height;
	};

	ImageData load(char const *const) const;

	ComPtr<ID3D11Texture2D> texture_ = nullptr;
	ComPtr<ID3D11ShaderResourceView> view_ = nullptr;
};