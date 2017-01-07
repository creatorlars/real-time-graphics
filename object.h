#pragma once

#include "model.h"
#include "matrix.h"

class direct3d;
class texture_shader;
class camera;

class object : public matrix
{
public:
	object(direct3d const&, char const *const, char const *const);

	virtual ~object() = default;

	explicit object(object const&) = default;
	explicit object(object&&) = default;

	object& operator=(object const&) = default;
	object& operator=(object&&) = default;

	inline std::shared_ptr<model> const& model() const
	{ return model_; }

	inline void model(std::shared_ptr<::model> const &model)
	{ model_ = model; }

	virtual XMMATRIX const render() const;

	virtual void render(std::shared_ptr<texture_shader> const&, std::shared_ptr<camera> const&)
	{}
	virtual void frame()
	{}

	inline unsigned index_count() const
	{ return model_->index_count(); }
	inline ComPtr<ID3D11ShaderResourceView> view() const
	{ return model_->view(); }

private:
	std::shared_ptr<::model> model_ = nullptr;
};