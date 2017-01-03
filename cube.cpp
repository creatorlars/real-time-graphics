#include "pch.h"
#include "cube.h"

#include "direct3d.h"

cube::cube(direct3d const& d3d)
	: d3d_(d3d), object(d3d, "data/cube.obj", "data/stone01.tga")
{
}

void cube::render()
{
	auto matrix = XMMatrixRotationY(rotation_);
	matrix *= XMMatrixRotationX(rotation_);
	matrix *= XMMatrixTranslation(5.f, 0.f, 0.f);
	XMStoreFloat4x4(&matrix_, matrix);

	model_.render();
}

void cube::frame()
{
	rotation_ += DirectX::XM_PI * 0.005f;
	if (rotation_ > 360.f)
	{
		rotation_ = 0.f;
	}
}