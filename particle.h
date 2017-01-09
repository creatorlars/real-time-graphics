#pragma once

#include "quad.h"
#include "matrix.h"

class direct3d;
class camera;
class shader;

class particle : public matrix//, public quad
{
public:
	particle(direct3d const&, ComPtr<ID3D11ShaderResourceView> const&);

	particle() = delete;
	virtual ~particle();

	explicit particle(particle const&) = default;
	explicit particle(particle&&) = default;

	particle& operator=(particle const&) = default;
	particle& operator=(particle&&) = default;

	virtual void frame() = 0;
	virtual bool alive() const = 0;

	template <typename T, typename... Args>
	void render(T const &shader, std::shared_ptr<camera> const &camera,
		Args... args) const
	{
		quad_->render();

		auto matrix = XMMatrixIdentity();

		// scale
		auto const s = scale();
		matrix *= XMMatrixScaling(s.x, s.y, s.z);

		// billboard rotation
		auto const p = position();
		auto const x = p.x - camera->position().x;
		auto const z = p.z - camera->position().z;
		auto const angle = static_cast<float>(std::atan2(x, z));
		matrix *= XMMatrixRotationY(angle);

		// translate
		matrix *= XMMatrixTranslation(p.x, p.y, p.z);

		shader->render(matrix, camera->render(), view_, quad_->index_count(), args...);
	}

	inline ComPtr<ID3D11ShaderResourceView> const& view() const
	{ return view_; }

private:
	ComPtr<ID3D11ShaderResourceView> const& view_;
	std::shared_ptr<quad> quad_ = nullptr;
};