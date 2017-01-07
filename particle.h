#pragma once

#include "quad.h"
#include "matrix.h"

class direct3d;
class camera;

class particle : public matrix, public quad
{
public:
	particle(direct3d const&, std::shared_ptr<camera> const&,
		ComPtr<ID3D11ShaderResourceView> const&);

	particle() = delete;
	virtual ~particle() = default;

	explicit particle(particle const&) = default;
	explicit particle(particle&&) = default;

	particle& operator=(particle const&) = default;
	particle& operator=(particle&&) = default;

	virtual void frame() = 0;
	virtual bool alive() const = 0;
	virtual XMMATRIX const render() const;

	inline ComPtr<ID3D11ShaderResourceView> const& view() const
	{ return view_; }

private:
	std::shared_ptr<camera> camera_ = nullptr;
	ComPtr<ID3D11ShaderResourceView> const& view_;
};