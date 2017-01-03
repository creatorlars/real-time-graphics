#include "pch.h"
#include "submarine.h"

#include "direct3d.h"

submarine::submarine(direct3d const& d3d)
	: d3d_(d3d), object(d3d, "data/submarine.obj", "data/stone01.tga")
{
}

void submarine::render()
{
	auto matrix = XMMatrixRotationY(rotation_);
	matrix *= XMMatrixRotationX(rotation_);
	XMStoreFloat4x4(&matrix_, matrix);

	model_.render();
}

void submarine::frame()
{
	rotation_ += DirectX::XM_PI * 0.005f;
	if (rotation_ > 360.f)
	{
		rotation_ = 0.f;
	}
}