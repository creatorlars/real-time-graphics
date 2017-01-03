#include "pch.h"
#include "submarine.h"

#include "direct3d.h"

submarine::submarine(direct3d const& d3d)
	: object(d3d, "data/submarine.obj", "data/stone01.tga"), d3d_(d3d)
{
}

void submarine::render()
{
	auto xmmatrix = XMMatrixRotationY(rotation_);
	xmmatrix *= XMMatrixRotationX(rotation_);
	auto fmatrix = XMFLOAT4X4{};
	XMStoreFloat4x4(&fmatrix, xmmatrix);
	matrix(fmatrix);

	model()->render();
}

void submarine::frame()
{
	rotation_ += DirectX::XM_PI * 0.005f;
	if (rotation_ > 360.f)
	{
		rotation_ = 0.f;
	}
}