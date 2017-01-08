#include "pch.h"
#include "submarine.h"

#include "direct3d.h"
#include "light_shader.h"

submarine::submarine(direct3d const& d3d,
	std::shared_ptr<light_shader> const &shader)
	: object(d3d, "data/submarine.obj", "data/blue.tga"), d3d_(d3d),
	shader_(shader)
{}

void submarine::frame()
{}