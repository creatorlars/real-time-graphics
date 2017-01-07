#include "pch.h"
#include "fish.h"

#include "direct3d.h"
#include "particle_emitter.h"
#include "texture_shader.h"
#include "camera.h"

fish::fish(direct3d const& d3d, std::shared_ptr<camera> const &camera)
	: object(d3d, "data/fish.obj", "data/orange.tga"), d3d_(d3d), camera_(camera)
{
	emitter_ = std::make_shared<particle_emitter>(d3d);

}

void fish::frame()
{
	emitter_->position(position());
	emitter_->frame();

	--wait_;
	if (wait_ == 0U)
	{
		wait_ = 20U;
		emitter_->emit(camera_);
	}
}

void fish::render(std::shared_ptr<texture_shader> const &shader,
	std::shared_ptr<camera> const &camera)
{
	emitter_->render(shader, camera);
}