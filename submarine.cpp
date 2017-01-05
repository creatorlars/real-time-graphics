#include "pch.h"
#include "submarine.h"

#include "direct3d.h"

submarine::submarine(direct3d const& d3d)
	: object(d3d, "data/submarine.obj", "data/stone.tga"), d3d_(d3d)
{}

void submarine::frame()
{
	rotate({ 1.f, 1.f, 0.f });
}