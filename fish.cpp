#include "pch.h"
#include "fish.h"

#include "direct3d.h"

fish::fish(direct3d const& d3d)
	: object(d3d, "data/fish.obj", "data/orange.tga"), d3d_(d3d)
{}

void fish::frame()
{
	rotate({ 1.f, 1.f, 0.f });
}