#pragma once

class ambient;
class spotlight;

class shader
{
public:
	shader() = default;
	~shader() = default;

	explicit shader(shader const&) = default;
	explicit shader(shader&&) = default;

	shader& operator=(shader const&) = default;
	shader& operator=(shader&&) = default;

	// texture shader
	virtual void render(XMMATRIX const&, XMMATRIX const&,
		ComPtr<ID3D11ShaderResourceView> const&, unsigned const) const
	{}

	// transparent shader
	virtual void render(XMMATRIX const&, XMMATRIX const&,
		ComPtr<ID3D11ShaderResourceView> const&, unsigned const, float const)
		const
	{}

	// light shader
	virtual void render(XMMATRIX const&, XMMATRIX const&,
		ComPtr<ID3D11ShaderResourceView> const&, unsigned const,
		std::shared_ptr<ambient> const&,
		std::vector<std::shared_ptr<spotlight>> const&) const
	{}
};