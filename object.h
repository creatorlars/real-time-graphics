#pragma once

#include "model.h"

class direct3d;

class object
{
public:
	object(direct3d const&, char const *const, char const *const);
	virtual ~object() = default;

	inline XMFLOAT4X4 const& matrix() const { return matrix_; }
	inline model const& model() const { return model_; }

	virtual void frame() = 0;
	virtual void render() = 0;

protected:
	XMFLOAT4X4 matrix_ = {};
	::model model_;
};