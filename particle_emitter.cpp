#include "pch.h"
#include "particle_emitter.h"

#include "transparent_shader.h"
#include "particle.h"
#include "bubble.h"
#include "camera.h"

particle_emitter::particle_emitter(direct3d const &d3d,
	std::shared_ptr<transparent_shader> const &shader)
	: texture_(d3d, "data/bubble.tga"), d3d_(d3d), shader_(shader)
{
}

void particle_emitter::frame()
{
	std::vector<std::vector<std::shared_ptr<particle>>::iterator> dead;
	for (auto it = particles_.begin(); it != particles_.end(); ++it)
	{
		auto const particle = (*it);
		if (particle->alive())
		{
			particle->frame();
		}
		else
		{
			dead.emplace_back(it);
		}
	}

	for (auto it = dead.crbegin(); it != dead.crend(); ++it)
	{
		particles_.erase(*it);
	}
}

void particle_emitter::render(std::shared_ptr<camera> const &camera)
{
	for (auto const &particle : particles_)
	{
		if (particle->alive())
		{
			particle->render(shader_, camera, .5f);
		}
	}
}

void particle_emitter::emit()
{
	particles_.emplace_back(std::make_shared<bubble>(d3d_, texture_.view(),
		position_, 500U));
}