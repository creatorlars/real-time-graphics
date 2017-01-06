#pragma once

#include "object.h"

class direct3d;
class particle_emitter;
class transparent_shader;
class camera;

class fish final : public object
{
public:
	explicit fish(direct3d const&);

	fish() = delete;
	~fish() = default;

	explicit fish(fish const&) = default;
	explicit fish(fish&&) = default;

	fish& operator=(fish const&) = default;
	fish& operator=(fish&&) = default;

	void frame() override;
	void render() override;
	void render(std::shared_ptr<transparent_shader> const&, std::shared_ptr<camera> const&) override;

private:
	direct3d const& d3d_;
	std::shared_ptr<particle_emitter> emitter_ = nullptr;

	unsigned wait_ = 20U;
};