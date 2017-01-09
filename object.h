#pragma once

#include "model.h"
#include "matrix.h"
#include "camera.h"

class direct3d;

class object : public matrix
{
public:
	object(direct3d const&, char const *const, char const *const);

	virtual ~object();

	explicit object(object const&) = default;
	explicit object(object&&) = default;

	object& operator=(object const&) = default;
	object& operator=(object&&) = default;

	inline std::shared_ptr<model> const& model() const
	{ return model_; }

	inline void model(std::shared_ptr<::model> const &model)
	{ model_ = model; }

	virtual void frame() = 0;
	virtual void render(std::shared_ptr<camera> const&) const = 0;

	inline unsigned index_count() const
	{ return model_->index_count(); }
	inline ComPtr<ID3D11ShaderResourceView> view() const
	{ return model_->view(); }

protected:
	template <typename T, typename... Args>
	void render_(T const &shader, std::shared_ptr<camera> const &camera,
		Args... args) const
	{
		model_->render();

		auto matrix = XMMatrixIdentity();

		// scale
		auto const s = scale();
		matrix *= XMMatrixScaling(s.x, s.y, s.z);

		// rotate
		auto const r = rotation();
		auto const pitch = XMConvertToRadians(r.x);
		auto const yaw = XMConvertToRadians(r.y);
		auto const roll = XMConvertToRadians(r.z);
		matrix *= XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

		// translate
		auto const p = position();
		matrix *= XMMatrixTranslation(p.x, p.y, p.z);

		shader->render(matrix, camera->render(), view(), index_count(), args...);
	}

private:
	std::shared_ptr<::model> model_ = nullptr;
};