#include "pch.h"
#include "cube.h"

#include "direct3d.h"

cube::cube(direct3d const& d3d)
	: object(d3d, "data/cube.obj", "data/stone.tga"), d3d_(d3d)
{}

void cube::frame()
{
	rotate({ -1.f, -1.f, 0.f });
}