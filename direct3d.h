#pragma once

class config;

class direct3d
{
public:
	direct3d(HWND const&, std::shared_ptr<config> const&);
	~direct3d();

	direct3d() = delete;

	explicit direct3d(direct3d const&) = default;
	explicit direct3d(direct3d&&) = default;

	direct3d& operator=(direct3d const&) = default;
	direct3d& operator=(direct3d&&) = default;

	void begin(float const, float const, float const, float const) const;
	void end() const;

	inline ComPtr<ID3D11Device> const& device() const
	{ return device_; }
	inline ComPtr<ID3D11DeviceContext> const& context() const
	{ return context_; }
	inline XMMATRIX const& projection_matrix() const
	{ return projection_matrix_; }
	inline XMMATRIX const& ortho_matrix() const
	{ return ortho_matrix_; }

	void enable_alpha_blending();
	void disable_alpha_blending();

private:
	bool vsync_;

	ComPtr<IDXGISwapChain> swap_chain_ = nullptr;
	ComPtr<ID3D11Device> device_ = nullptr;
	ComPtr<ID3D11DeviceContext> context_ = nullptr;
	ComPtr<ID3D11RenderTargetView> render_target_view_ = nullptr;
	ComPtr<ID3D11Texture2D> depth_stencil_buffer_ = nullptr;
	ComPtr<ID3D11DepthStencilState> depth_stencil_state_ = nullptr;
	ComPtr<ID3D11DepthStencilView> depth_stencil_view_ = nullptr;
	ComPtr<ID3D11RasterizerState> raster_state_ = nullptr;

	XMMATRIX projection_matrix_ = {};
	XMMATRIX ortho_matrix_ = {};
};