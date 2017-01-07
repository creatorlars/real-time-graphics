#include "pch.h"
#include "drebbel.h"

#include "direct3d.h"

drebbel::drebbel(direct3d const& d3d)
	: object(d3d, "data/drebbel.obj", "data/blue.tga"), d3d_(d3d)
{}

void drebbel::frame()
{}