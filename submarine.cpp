#include "pch.h"
#include "submarine.h"

#include "direct3d.h"

submarine::submarine(direct3d const& d3d)
	: object(d3d, "data/new_submarine.obj", "data/blue.tga"), d3d_(d3d)
{}

void submarine::frame()
{}