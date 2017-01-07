#pragma once

class direct3d;
class object;
class camera;
class light;

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

	template<typename T>
	void render(T const &object, std::shared_ptr<camera> const &camera,
		std::vector<std::shared_ptr<light>> const &lights) const
	{
		render(object->render(), camera->render(), object->view(),
			object->index_count(), lights);
	}

private:
	void render(XMMATRIX const&, XMMATRIX const&,
		ComPtr<ID3D11ShaderResourceView> const&, unsigned const,
		std::vector<std::shared_ptr<light>> const&) const;

	ComPtr<ID3D11VertexShader> vertex_shader_ = nullptr;
	ComPtr<ID3D11PixelShader> pixel_shader_ = nullptr;
	ComPtr<ID3D11InputLayout> layout_ = nullptr;
	ComPtr<ID3D11SamplerState> sampler_state_ = nullptr;
	ComPtr<ID3D11Buffer> matrix_buffer_ = nullptr;
	ComPtr<ID3D11Buffer> light_buffer_ = nullptr;
	ComPtr<ID3D11Buffer> light_position_buffer_ = nullptr;

	direct3d const& d3d_;
};