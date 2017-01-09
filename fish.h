#pragma once

#include "object.h"

class direct3d;
class particle_emitter;
class transparent_shader;
class light_shader;

class shader;

class fish final : public object
{
public:
	fish(direct3d const&, std::shared_ptr<transparent_shader> const&,
		std::shared_ptr<light_shader> const&);

	fish() = delete;
	~fish() = default;

	explicit fish(fish const&) = default;
	explicit fish(fish&&) = default;

	fish& operator=(fish const&) = default;
	fish& operator=(fish&&) = default;

	void frame() override;
	void render(std::shared_ptr<camera> const&) const override;

private:
	direct3d const& d3d_;
	std::shared_ptr<light_shader> shader_ = nullptr;
	std::shared_ptr<particle_emitter> emitter_ = nullptr;

	unsigned wait_ = 20U;
};