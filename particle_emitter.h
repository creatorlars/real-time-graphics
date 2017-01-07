#pragma once

#include "texture.h"

class texture_shader;
class camera;
class particle;
class direct3d;

class particle_emitter
{
public:
	particle_emitter(direct3d const&);

	~particle_emitter() = default;

	particle_emitter(particle_emitter const&) = default;
	particle_emitter(particle_emitter&&) = default;

	particle_emitter& operator=(particle_emitter const&) = default;
	particle_emitter& operator=(particle_emitter&&) = default;

	void frame();
	void render(std::shared_ptr<texture_shader> const&, std::shared_ptr<camera> const&);
	void emit(std::shared_ptr<camera> const&);

	inline XMFLOAT3 const& position()
	{ return position_; }

	inline void position(XMFLOAT3 const &position)
	{ position_ = position; }

private:
	std::vector<std::shared_ptr<particle>> particles_ = {};
	texture texture_;

	XMFLOAT3 position_ = {};

	direct3d const &d3d_;
};