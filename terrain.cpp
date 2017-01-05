#include "pch.h"
#include "terrain.h"

#include "direct3d.h"

terrain::terrain(direct3d const& d3d)
	: object(d3d, "data/terrain.obj", "data/sand.tga"), d3d_(d3d)
{}