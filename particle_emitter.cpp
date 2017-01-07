#include "pch.h"
#include "particle_emitter.h"

#include "texture_shader.h"
#include "bubble.h"
#include "camera.h"

particle_emitter::particle_emitter(direct3d const &d3d)
	: texture_(d3d, "data/bubble.tga"), d3d_(d3d)
{
}

void particle_emitter::frame()
{
	std::vector<std::vector<std::shared_ptr<bubble>>::iterator> dead;
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

void particle_emitter::render(std::shared_ptr<texture_shader> const &shader,
	std::shared_ptr<camera> const &camera)
{
	for (auto const &particle : particles_)
	{
		if (particle->alive())
		{
			shader->render(particle, camera);
		}
	}
}

void particle_emitter::emit(std::shared_ptr<camera> const &camera)
{
	particles_.emplace_back(std::make_shared<bubble>(d3d_, texture_, position_,
		camera, 1'000U));
}