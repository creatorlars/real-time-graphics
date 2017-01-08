#pragma once

#include "particle.h"

class direct3d;

class bubble final : public particle
{
public:
	bubble(direct3d const&, ComPtr<ID3D11ShaderResourceView> const&,
		XMFLOAT3 const&, unsigned const);

	void frame() override;

	inline bool alive() const override
	{ return life_ != 0U; }

private:
	unsigned life_ = 0U;
};