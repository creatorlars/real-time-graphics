#pragma once

#include "texture.h"

class transparent_shader;
class camera;
class particle;
class direct3d;

class particle_emitter
{
public:
	particle_emitter(direct3d const&, std::shared_ptr<transparent_shader> const&);

	~particle_emitter() = default;

	particle_emitter(particle_emitter const&) = default;
	particle_emitter(particle_emitter&&) = default;

	particle_emitter& operator=(particle_emitter const&) = default;
	particle_emitter& operator=(particle_emitter&&) = default;

	void frame();
	void render(std::shared_ptr<camera> const&);
	void emit();

	inline XMFLOAT3 const& position()
	{ return position_; }

	inline void position(XMFLOAT3 const &position)
	{ position_ = position; }

private:
	std::shared_ptr<transparent_shader> shader_ = nullptr;
	std::vector<std::shared_ptr<particle>> particles_ = {};
	texture texture_;

	XMFLOAT3 position_ = {};

	direct3d const &d3d_;
};