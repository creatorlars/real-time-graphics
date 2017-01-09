#pragma once

class direct3d;
class ambient;
class spotlight;

class light_shader
{
public:
	explicit light_shader(direct3d const&);

	light_shader() = delete;
	~light_shader() = default;

	explicit light_shader(light_shader const&) = default;
	explicit light_shader(light_shader&&) = default;

	light_shader& operator=(light_shader const&) = default;
	light_shader& operator=(light_shader&&) = default;

	inline void begin_render(std::shared_ptr<ambient> const &ambient,
		std::vector<std::shared_ptr<spotlight>> const &spotlights,
		float const exposure, float const gamma)
	{
		ambient_ = ambient;
		spotlights_ = spotlights;
		exposure_ = exposure;
		gamma_ = gamma;
	}

	void render(XMMATRIX const&, XMMATRIX const&,
		ComPtr<ID3D11ShaderResourceView> const&, unsigned const) const;

private:
	ComPtr<ID3D11VertexShader> vertex_shader_ = nullptr;
	ComPtr<ID3D11PixelShader> pixel_shader_ = nullptr;
	ComPtr<ID3D11InputLayout> layout_ = nullptr;
	ComPtr<ID3D11SamplerState> sampler_state_ = nullptr;
	ComPtr<ID3D11Buffer> matrix_buffer_ = nullptr;
	ComPtr<ID3D11Buffer> light_buffer_ = nullptr;
	ComPtr<ID3D11Buffer> light_position_buffer_ = nullptr;

	std::shared_ptr<ambient> ambient_ = nullptr;
	std::vector<std::shared_ptr<spotlight>> spotlights_ = {};
	float exposure_ = 1.f;
	float gamma_ = 1.f;

	direct3d const& d3d_;
};