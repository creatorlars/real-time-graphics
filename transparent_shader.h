#pragma once

class direct3d;
class object;
class camera;

class transparent_shader
{
public:
	explicit transparent_shader(direct3d const&);

	transparent_shader() = default;
	~transparent_shader() = default;

	explicit transparent_shader(transparent_shader const&) = default;
	explicit transparent_shader(transparent_shader&&) = default;

	transparent_shader& operator=(transparent_shader const&) = default;
	transparent_shader& operator=(transparent_shader&&) = default;

	void render(std::shared_ptr<object> const&, std::shared_ptr<camera> const&, float) const;

private:
	ComPtr<ID3D11VertexShader> vertex_shader_ = nullptr;
	ComPtr<ID3D11PixelShader> pixel_shader_ = nullptr;
	ComPtr<ID3D11InputLayout> layout_ = nullptr;
	ComPtr<ID3D11Buffer> matrix_buffer_ = nullptr;
	ComPtr<ID3D11SamplerState> sample_state_ = nullptr;
	ComPtr<ID3D11Buffer> transparent_buffer_ = nullptr;

	direct3d const& d3d_;
};