#pragma once

class direct3d;
class object;
class camera;

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

	void render(std::shared_ptr<object> const&, std::shared_ptr<camera> const&) const;

private:
	ComPtr<ID3D11VertexShader> vertex_shader_ = nullptr;
	ComPtr<ID3D11PixelShader> pixel_shader_ = nullptr;
	ComPtr<ID3D11InputLayout> layout_ = nullptr;
	ComPtr<ID3D11SamplerState> sampler_state_ = nullptr;
	ComPtr<ID3D11Buffer> matrix_buffer_ = nullptr;
	ComPtr<ID3D11Buffer> light_buffer_ = nullptr;

	direct3d const& d3d_;
};