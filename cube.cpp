#include "pch.h"
#include "cube.h"

#include "direct3d.h"
#include "light_shader.h"

cube::cube(direct3d const &d3d, std::shared_ptr<light_shader> const &shader)
	: object(d3d, "data/cube.obj", "data/sand.tga"), d3d_(d3d), shader_(shader)
{}

cube::~cube()
{}

void cube::frame()
{
	rotate({ -1.f, -1.f, 0.f });
}

void cube::render(std::shared_ptr<camera> const &camera) const
{
	render_(shader_, camera);
}