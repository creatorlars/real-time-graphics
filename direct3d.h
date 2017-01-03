#pragma once

class direct3d
{
public:
	direct3d(unsigned, unsigned, bool, HWND, bool, float, float);
	~direct3d();

	void begin(float, float, float, float);
	void end();

	inline ComPtr<ID3D11Device> device() const { return device_; }
	inline ComPtr<ID3D11DeviceContext> context() const { return context_; }
	inline XMFLOAT4X4 const& projection_matrix() const { return projection_matrix_; }
	inline XMFLOAT4X4 const& world_matrix() const { return world_matrix_; }
	inline XMFLOAT4X4 const& ortho_matrix() const { return ortho_matrix_; }

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

	XMFLOAT4X4 projection_matrix_;
	XMFLOAT4X4 world_matrix_;
	XMFLOAT4X4 ortho_matrix_;
};