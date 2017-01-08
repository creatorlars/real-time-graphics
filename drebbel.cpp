#include "pch.h"
#include "drebbel.h"

#include "direct3d.h"
#include "light_shader.h"

drebbel::drebbel(direct3d const& d3d,
	std::shared_ptr<light_shader> const &shader)
	: object(d3d, "data/drebbel.obj", "data/blue.tga"), d3d_(d3d),
	shader_(shader)
{}

void drebbel::frame()
{}