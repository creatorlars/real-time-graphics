#include "pch.h"
#include "submarine.h"

#include "direct3d.h"
#include "light_shader.h"

submarine::submarine(direct3d const& d3d,
	std::shared_ptr<light_shader> const &shader)
	: object(d3d, "data/submarine.obj", "data/blue.tga"), d3d_(d3d),
	shader_(shader), propeller_(std::make_shared<propeller>(d3d, shader))
{}

submarine::~submarine()
{}

void submarine::frame()
{
	auto const centre = position();
	propeller_->position({ centre.x, centre.y, centre.z + 1.5f });
	propeller_->frame();
}

void submarine::render(std::shared_ptr<camera> const &camera) const
{
	render_(shader_, camera);
	propeller_->render(camera);
}

submarine::propeller::propeller(direct3d const &d3d,
	std::shared_ptr<light_shader> const &shader)
	: object(d3d, "data/propeller.obj", "data/blue.tga"), d3d_(d3d),
	shader_(shader)
{
	scale({ .1f, .1f, .1f });
}

submarine::propeller::~propeller()
{
}

void submarine::propeller::frame()
{
	rotate_clockwise(1.f);
}

void submarine::propeller::render(std::shared_ptr<camera> const &camera) const
{
	render_(shader_, camera);
}