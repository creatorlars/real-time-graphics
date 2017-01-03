#pragma once

class direct3d;
class object;
class camera;

class texture_shader
{
public:
	texture_shader(direct3d const&);

	void render(std::shared_ptr<object> const&, camera const&) const;

private:
	ComPtr<ID3D11VertexShader> vertex_shader_ = nullptr;
	ComPtr<ID3D11PixelShader> pixel_shader_ = nullptr;
	ComPtr<ID3D11InputLayout> layout_ = nullptr;
	ComPtr<ID3D11Buffer> matrix_buffer_ = nullptr;
	ComPtr<ID3D11SamplerState> sample_state_ = nullptr;

	direct3d const& d3d_;
};