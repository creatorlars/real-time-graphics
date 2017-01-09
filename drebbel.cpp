#include "pch.h"
#include "drebbel.h"

#include "direct3d.h"
#include "light_shader.h"

drebbel::drebbel(direct3d const& d3d,
	std::shared_ptr<light_shader> const &shader)
	: object(d3d, "data/drebbel.obj", "data/blue.tga"), d3d_(d3d),
	shader_(shader)
{
	for (auto i = size_t{}; i < oars_.size(); ++i)
	{
		oars_[i] = std::make_shared<oar>(d3d, shader);
	}
}

drebbel::~drebbel()
{
}

void drebbel::frame()
{
	auto const centre = position();
	oars_[0]->rotation({ 0.f, 90.f, 90.f });
	oars_[0]->position({ centre.x, centre.y, centre.z });
	oars_[0]->frame();

	oars_[1]->rotation({ 0.f, -90.f, 90.f });
	oars_[1]->position({ centre.x, centre.y, centre.z });
	oars_[1]->frame();
}

void drebbel::render(std::shared_ptr<camera> const &camera) const
{
	render_(shader_, camera);
	for (auto const &it : oars_)
	{
		it->render(camera);

	}
}

drebbel::oar::oar(direct3d const &d3d,
	std::shared_ptr<light_shader> const &shader)
	: object(d3d, "data/oar.obj", "data/blue.tga"), d3d_(d3d), shader_(shader)
{
	scale({ .2f, .2f, .2f });
}

drebbel::oar::~oar()
{
}

void drebbel::oar::frame()
{
	static auto step = 0U;
	if (step % 2)
	{
		auto const offset = std::sinf(step / 100.f);
		rotate_left(offset * 10.f);
		rotate_clockwise(offset * 10.f);
		++step;
	}
	else
	{
		auto const offset = -std::sinf(step / 100.f);
		rotate_left(offset * 10.f);
		rotate_clockwise(offset * 10.f);
		++step;
	}	
}

void drebbel::oar::render(std::shared_ptr<camera> const &camera) const
{
	render_(shader_, camera);
}