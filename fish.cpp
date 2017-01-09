#include "pch.h"
#include "fish.h"

#include "direct3d.h"
#include "particle_emitter.h"
#include "texture_shader.h"
#include "light_shader.h"
#include "camera.h"

fish::fish(direct3d const& d3d,
	std::shared_ptr<transparent_shader> const &emitter_shader,
	std::shared_ptr<light_shader> const &shader)
	: object(d3d, "data/fish.obj", "data/orange.tga"), d3d_(d3d),
	shader_(shader)
{
	emitter_ = std::make_shared<particle_emitter>(d3d, emitter_shader);
}

void fish::frame()
{
	emitter_->position(position());
	emitter_->frame();

	--wait_;
	if (wait_ == 0U)
	{
		wait_ = 20U;
		emitter_->emit();
	}
}

void fish::render(std::shared_ptr<camera> const &camera) const
{
	emitter_->render(camera);
	object::render(shader_, camera);
}