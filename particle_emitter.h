#pragma once

class particle;

class particle_emitter
{
public:
	particle_emitter() = default;
	~particle_emitter() = default;

	particle_emitter(particle_emitter const&) = default;
	particle_emitter(particle_emitter&&) = default;

	particle_emitter& operator=(particle_emitter const&) = default;
	particle_emitter& operator=(particle_emitter&&) = default;

private:
	std::vector<std::shared_ptr<particle>> particles_ = {};
};