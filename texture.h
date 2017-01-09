#pragma once

class direct3d;

class texture
{
public:
	texture(direct3d const&, char const *const);

	texture() = delete;
	~texture() = default;

	explicit texture(texture const&) = default;
	explicit texture(texture&&) = default;

	texture& operator=(texture const&) = default;
	texture& operator=(texture&&) = default;

	inline ComPtr<ID3D11ShaderResourceView> const& view() const
	{ return view_; }

private:
	ComPtr<ID3D11Texture2D> texture_ = nullptr;
	ComPtr<ID3D11ShaderResourceView> view_ = nullptr;
};