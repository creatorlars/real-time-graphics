#include "pch.h"
#include "sphere.h"

#include "direct3d.h"

sphere::sphere(direct3d const& d3d)
	: object(d3d, "data/sphere.obj", "data/blue.tga"), d3d_(d3d)
{}