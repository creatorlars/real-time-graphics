#include "pch.h"
#include "cube.h"

#include "direct3d.h"

cube::cube(direct3d const& d3d)
	: object(d3d, "data/cube.obj", "data/stone01.tga"), d3d_(d3d)
{
}

void cube::render()
{
	auto xmmatrix = XMMatrixRotationY(rotation_);
	xmmatrix *= XMMatrixRotationX(rotation_);
	xmmatrix *= XMMatrixTranslation(5.f, 0.f, 0.f);
	auto fmatrix = XMFLOAT4X4{};
	XMStoreFloat4x4(&fmatrix, xmmatrix);
	matrix(fmatrix);

	model()->render();
}

void cube::frame()
{
	rotation_ += DirectX::XM_PI * 0.005f;
	if (rotation_ > 360.f)
	{
		rotation_ = 0.f;
	}
}