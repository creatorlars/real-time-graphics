#include "pch.h"
#include "sphere.h"

#include "direct3d.h"

sphere::sphere(direct3d const& d3d)
	: d3d_(d3d), object(d3d, "data/sphere.obj", "data/blue.tga")
{
}

void sphere::render()
{
	auto const matrix = XMMatrixScaling(5.f, 5.f, 5.f);
	XMStoreFloat4x4(&matrix_, matrix);

	model_.render();
}

void sphere::frame()
{
}