#pragma once

#include "model.h"

class direct3d;

class object
{
public:
	object(direct3d const&, char const *const, char const *const);
	virtual ~object() = default;

	explicit object(object const&) = default;
	explicit object(object&&) = default;

	object& operator=(object const&) = default;
	object& operator=(object&&) = default;

	inline XMFLOAT4X4 const& matrix() const { return matrix_; }
	inline std::shared_ptr<model> const& model() const { return model_; }

	inline void matrix(XMFLOAT4X4 const &matrix) { matrix_ = matrix; }
	inline void model(std::shared_ptr<::model> const &model) { model_ = model; }

	virtual void frame() = 0;
	virtual void render() = 0;

private:
	XMFLOAT4X4 matrix_ = {};
	std::shared_ptr<::model> model_ = nullptr;
};