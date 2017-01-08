#include "pch.h"
#include "terrain.h"

#include "direct3d.h"
#include "light_shader.h"

terrain::terrain(direct3d const& d3d,
	std::shared_ptr<light_shader> const &shader)
	: object(d3d, "data/terrain.obj", "data/sand.tga"), d3d_(d3d),
	shader_(shader)
{}