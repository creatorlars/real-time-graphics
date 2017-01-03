#pragma once

class direct3d;
class object;
class camera;

class transparent_shader
{
public:
	transparent_shader(direct3d const&);

	void render(std::shared_ptr<object> const&, camera const&, float) const;

private:
	ComPtr<ID3D11VertexShader> vertex_shader_ = nullptr;
	ComPtr<ID3D11PixelShader> pixel_shader_ = nullptr;
	ComPtr<ID3D11InputLayout> layout_ = nullptr;
	ComPtr<ID3D11Buffer> matrix_buffer_ = nullptr;
	ComPtr<ID3D11SamplerState> sample_state_ = nullptr;
	ComPtr<ID3D11Buffer> transparent_buffer_ = nullptr;

	direct3d const& d3d_;
};