#include "pch.h"
#include "direct3d.h"

direct3d::direct3d(unsigned screenWidth, unsigned screenHeight, bool vsync,
	HWND hwnd, bool fullscreen, float screenDepth, float screenNear)
{
	HRESULT result{};

	// Store the vsync setting.
	vsync_ = vsync;

	// Create a DirectX graphics interface factory.
	auto factory = ComPtr<IDXGIFactory>{};
	result = CreateDXGIFactory(__uuidof(IDXGIFactory),
		reinterpret_cast<void**>(factory.GetAddressOf()));
	if (FAILED(result))
	{
		throw "";
	}

	// Use the factory to create an adapter for the primary graphics interface
	// (video card).
	auto adapter = ComPtr<IDXGIAdapter>{};
	result = factory->EnumAdapters(0U, &adapter);
	if (FAILED(result))
	{
		throw "";
	}

	// Enumerate the primary adapter output (monitor).
	auto output = ComPtr<IDXGIOutput>{};
	result = adapter->EnumOutputs(0U, &output);
	if (FAILED(result))
	{
		throw "";
	}

	// Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display
	// format for the adapter output (monitor).
	auto count = unsigned{};
	result = output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_ENUM_MODES_INTERLACED, &count, NULL);
	if (FAILED(result))
	{
		throw "";
	}

	// Now fill the display mode list structures.
	auto display_modes = std::vector<DXGI_MODE_DESC>(count);
	result = output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_ENUM_MODES_INTERLACED, &count, display_modes.data());
	if (FAILED(result))
	{
		throw "";
	}

	// Now go through all the display modes and find the one that matches the
	// screen width and height. When a match is found store the numerator and
	// denominator of the refresh rate for that monitor.
	auto refresh_rate = DXGI_RATIONAL{ 0U, 1U };
	for (auto const &it : display_modes)
	{
		if (it.Width == screenWidth && it.Height == screenHeight)
		{
			refresh_rate = it.RefreshRate;
		}
	}

	auto const swap_chain_desc = DXGI_SWAP_CHAIN_DESC
	{
		{
			screenWidth, screenHeight,
			refresh_rate,
			DXGI_FORMAT_R8G8B8A8_UNORM,
			DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED,
			DXGI_MODE_SCALING_UNSPECIFIED
		},
		{
			1U,
			0U
		},
		DXGI_USAGE_RENDER_TARGET_OUTPUT,
		1U,
		hwnd,
		!fullscreen,
		DXGI_SWAP_EFFECT_DISCARD,
		0U
	};

	// Create the swap chain, Direct3D device, and Direct3D device context.
	auto constexpr feature_level = D3D_FEATURE_LEVEL_11_0;
	result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL,
		0U, &feature_level, 1U, D3D11_SDK_VERSION, &swap_chain_desc,
		swap_chain_.GetAddressOf(), device_.GetAddressOf(), NULL,
		context_.GetAddressOf());
	if (FAILED(result))
	{
		throw "";
	}

	// Get the pointer to the back buffer.
	auto back_buffer = ComPtr<ID3D11Texture2D>{};
	result = swap_chain_->GetBuffer(0U, __uuidof(ID3D11Texture2D),
		reinterpret_cast<LPVOID*>(back_buffer.GetAddressOf()));
	if (FAILED(result))
	{
		throw "";
	}

	// Create the render target view with the back buffer pointer.
	result = device_->CreateRenderTargetView(back_buffer.Get(), NULL,
		&render_target_view_);
	if (FAILED(result))
	{
		throw "";
	}

	auto const depth_buffer_desc = D3D11_TEXTURE2D_DESC
	{
		screenWidth, screenHeight,
		1U,
		1U,
		DXGI_FORMAT_D24_UNORM_S8_UINT,
		{
			1U,
			0U
		},
		D3D11_USAGE_DEFAULT,
		D3D11_BIND_DEPTH_STENCIL,
		0U, 0U
	};

	// Create the texture for the depth buffer using the filled out description.
	result = device_->CreateTexture2D(&depth_buffer_desc, NULL,
		depth_stencil_buffer_.GetAddressOf());
	if (FAILED(result))
	{
		throw "";
	}

	auto constexpr depth_stencil_desc = D3D11_DEPTH_STENCIL_DESC
	{
		TRUE,
		D3D11_DEPTH_WRITE_MASK_ALL,
		D3D11_COMPARISON_LESS,
		TRUE,
		0xFF, 0xFF,
		{
			D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_INCR, D3D11_STENCIL_OP_KEEP,
			D3D11_COMPARISON_ALWAYS
		},
		{
			D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_INCR, D3D11_STENCIL_OP_KEEP,
			D3D11_COMPARISON_ALWAYS
		}
	};

	// Create the depth stencil state.
	result = device_->CreateDepthStencilState(&depth_stencil_desc,
		depth_stencil_state_.GetAddressOf());
	if (FAILED(result))
	{
		throw "";
	}

	// Set the depth stencil state.
	context_->OMSetDepthStencilState(depth_stencil_state_.Get(), 1);

	auto constexpr depth_stencil_view_desc = D3D11_DEPTH_STENCIL_VIEW_DESC
	{
		DXGI_FORMAT_D24_UNORM_S8_UINT,
		D3D11_DSV_DIMENSION_TEXTURE2D,
		0U,
		0U
	};

	// Create the depth stencil view.
	result = device_->CreateDepthStencilView(depth_stencil_buffer_.Get(),
		&depth_stencil_view_desc, depth_stencil_view_.GetAddressOf());
	if (FAILED(result))
	{
		throw "";
	}

	// Bind the render target view and depth stencil buffer to the output render pipeline.
	context_->OMSetRenderTargets(1U, render_target_view_.GetAddressOf(),
		depth_stencil_view_.Get());

	auto constexpr rasterizer_desc = D3D11_RASTERIZER_DESC
	{
		D3D11_FILL_SOLID, D3D11_CULL_BACK,
		FALSE,
		0, .0f, .0f, TRUE,
		FALSE, FALSE, FALSE
	};

	// Create the rasterizer state from the description we just filled out.
	result = device_->CreateRasterizerState(&rasterizer_desc,
		raster_state_.GetAddressOf());
	if (FAILED(result))
	{
		throw "";
	}

	// Now set the rasterizer state.
	context_->RSSetState(raster_state_.Get());

	auto const width_f = static_cast<float>(screenWidth);
	auto const height_f = static_cast<float>(screenHeight);

	auto const viewport = D3D11_VIEWPORT
	{
		.0f, .0f,
		width_f, height_f,
		.0f, 1.f
	};

	// Create the viewport.
	context_->RSSetViewports(1U, &viewport);

	// Create the projection matrix for 3D rendering.
	auto const projection_matrix = XMMatrixPerspectiveFovLH(XM_PI / 4.f,
		width_f / height_f, screenNear, screenDepth);
	XMStoreFloat4x4(&projection_matrix_, projection_matrix);

	// Initialize the world matrix to the identity matrix.
	auto const world_matrix = XMMatrixIdentity();
	XMStoreFloat4x4(&world_matrix_, world_matrix);

	// Create an orthographic projection matrix for 2D rendering.
	auto const ortho_matrix = XMMatrixOrthographicLH(width_f, height_f,
		screenNear, screenDepth);
	XMStoreFloat4x4(&ortho_matrix_, ortho_matrix);
}

direct3d::~direct3d()
{
	// Before shutting down set to windowed mode or when you release the swap
	// chain it will throw an exception.
	if (swap_chain_)
	{
		swap_chain_->SetFullscreenState(FALSE, NULL);
	}
}

void direct3d::begin(float r, float g, float b, float a)
{
	auto const colour = std::array<float, 4> {r, g, b, a};

	// Clear the back buffer.
	context_->ClearRenderTargetView(render_target_view_.Get(), colour.data());

	// Clear the depth buffer. 
	context_->ClearDepthStencilView(depth_stencil_view_.Get(), D3D11_CLEAR_DEPTH, 1.f,
		0U);
}

void direct3d::end()
{
	if (vsync_)
	{
		swap_chain_->Present(1U, 0U);
	}
	else
	{
		swap_chain_->Present(0U, 0U);
	}
}

void direct3d::enable_alpha_blending()
{
	auto constexpr blend_description = D3D11_BLEND_DESC
	{
		FALSE, FALSE,
		TRUE,
		D3D11_BLEND_SRC_ALPHA, D3D11_BLEND_INV_SRC_ALPHA, D3D11_BLEND_OP_ADD,
		D3D11_BLEND_ONE, D3D11_BLEND_ZERO, D3D11_BLEND_OP_ADD,
		0x0F
	};

	static ID3D11BlendState *blend_state = nullptr;
	if (nullptr == blend_state)
	{
		device_->CreateBlendState(&blend_description, &blend_state);
	}

	auto constexpr factor = std::array<float, 4U>{};
	context_->OMSetBlendState(blend_state, factor.data(), 0xFFFFFFFF);
}

void direct3d::disable_alpha_blending()
{
	auto constexpr blend_description = D3D11_BLEND_DESC
	{
		FALSE, FALSE,
		FALSE,
		D3D11_BLEND_SRC_ALPHA, D3D11_BLEND_INV_SRC_ALPHA, D3D11_BLEND_OP_ADD,
		D3D11_BLEND_ONE, D3D11_BLEND_ZERO, D3D11_BLEND_OP_ADD,
		0x0F
	};

	static ID3D11BlendState *blend_state = nullptr;
	if (nullptr == blend_state)
	{
		device_->CreateBlendState(&blend_description, &blend_state);
	}

	auto constexpr factor = std::array<float, 4U>{};
	context_->OMSetBlendState(blend_state, factor.data(), 0xFFFFFFFF);
}