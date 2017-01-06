#pragma once

#include "model.h"

class direct3d;
class transparent_shader;
class camera;

class object
{
public:
	object(direct3d const&, char const *const, char const *const);

	virtual ~object() = default;

	explicit object(object const&) = default;
	explicit object(object&&) = default;

	object& operator=(object const&) = default;
	object& operator=(object&&) = default;

	inline XMFLOAT4X4 const& matrix() const
	{ return matrix_; }
	inline std::shared_ptr<model> const& model() const
	{ return model_; }
	inline XMFLOAT3 const& scale() const
	{ return scale_; }
	inline XMFLOAT3 const& rotation() const
	{ return rotation_; }
	inline XMFLOAT3 const& position() const
	{ return position_;	}

	inline void matrix(XMFLOAT4X4 const &matrix)
	{ matrix_ = matrix; }
	inline void model(std::shared_ptr<::model> const &model)
	{ model_ = model; }
	inline void scale(XMFLOAT3 const& scale)
	{ scale_ = scale; }
	inline void rotation(XMFLOAT3 const& rotation)
	{ rotation_ = rotation; }
	inline void position(XMFLOAT3 const& position)
	{ position_ = position; }

	virtual void render();
	virtual void render(std::shared_ptr<transparent_shader> const&, std::shared_ptr<camera> const&)
	{}
	virtual void frame()
	{}

	void rotate(XMFLOAT3 const&);
	void move(XMFLOAT3 const&);

	inline unsigned index_count() const
	{ return model_->index_count(); }
	inline ComPtr<ID3D11ShaderResourceView> view() const
	{ return model_->view(); }

private:
	XMFLOAT4X4 matrix_ = {};
	std::shared_ptr<::model> model_ = nullptr;

	XMFLOAT3 scale_ = {1.f, 1.f, 1.f};
	XMFLOAT3 rotation_ = {};
	XMFLOAT3 position_ = {};
};