#pragma once

class direct3d;
class object;
class camera;

class texture_shader
{
public:
	explicit texture_shader(direct3d const&);

	texture_shader() = default;
	~texture_shader() = default;

	explicit texture_shader(texture_shader const&) = default;
	explicit texture_shader(texture_shader&&) = default;

	texture_shader& operator=(texture_shader const&) = default;
	texture_shader& operator=(texture_shader&&) = default;

	void render(std::shared_ptr<object> const&, std::shared_ptr<camera> const&) const;

private:
	ComPtr<ID3D11VertexShader> vertex_shader_ = nullptr;
	ComPtr<ID3D11PixelShader> pixel_shader_ = nullptr;
	ComPtr<ID3D11InputLayout> layout_ = nullptr;
	ComPtr<ID3D11Buffer> matrix_buffer_ = nullptr;
	ComPtr<ID3D11SamplerState> sample_state_ = nullptr;

	direct3d const& d3d_;
};