#include "pch.h"
#include "sphere.h"

#include "direct3d.h"
#include "transparent_shader.h"

sphere::sphere(direct3d const& d3d,
	std::shared_ptr<transparent_shader> const &shader)
	: object(d3d, "data/sphere.obj", "data/blue.tga"), d3d_(d3d),
	shader_(shader)
{}

sphere::~sphere()
{
}

void sphere::frame()
{}

void sphere::render(std::shared_ptr<camera> const &camera) const
{
	render_(shader_, camera, .25f);
}