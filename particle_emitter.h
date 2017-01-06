#pragma once

#include "texture.h"
#include "quad.h"

class transparent_shader;
class camera;

class bubble
{
public:
	bubble(direct3d const&, texture const&, XMFLOAT3 const&, unsigned const);

	void frame();
	void render();

	inline XMFLOAT4X4 const& matrix()
	{ return matrix_; }
	inline unsigned const index_count() const
	{ return quad_.index_count(); }
	inline ComPtr<ID3D11ShaderResourceView> const& view() const
	{ return texture_.view(); }
	inline bool alive() const
	{ return life_ != 0U; }

	inline void matrix(XMFLOAT4X4 const &matrix)
	{ matrix_ = matrix; }

	void move(XMFLOAT3 const&);

private:
	XMFLOAT4X4 matrix_ = {};
	XMFLOAT3 position_ = {};
	XMFLOAT3 scale_ = { .01f, .01f, .01f };

	texture const &texture_;
	unsigned life_ = 0U;

	quad quad_;
};

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
	void render(std::shared_ptr<transparent_shader> const&, std::shared_ptr<camera> const&);
	void emit();

	inline XMFLOAT3 const& position()
	{ return position_; }

	inline void position(XMFLOAT3 const &position)
	{ position_ = position; }

private:
	std::vector<std::shared_ptr<bubble>> particles_ = {};
	texture texture_;

	XMFLOAT3 position_ = {};

	direct3d const &d3d_;
};