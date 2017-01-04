#include "pch.h"
#include "sphere.h"

#include "direct3d.h"

sphere::sphere(direct3d const& d3d)
	: object(d3d, "data/sphere.obj", "data/blue.tga"), d3d_(d3d)
{
	auto const xmmatrix = XMMatrixIdentity() + XMMatrixScaling(10.f, 10.f, 10.f);
	auto fmatrix = XMFLOAT4X4{};
	XMStoreFloat4x4(&fmatrix, xmmatrix);
	matrix(fmatrix);
}

void sphere::render()
{
	model()->render();
}

void sphere::frame()
{
}