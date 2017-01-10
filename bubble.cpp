#include "pch.h"
#include "bubble.h"

#include "camera.h"

bubble::bubble(direct3d const &d3d,
	ComPtr<ID3D11ShaderResourceView> const &view, XMFLOAT3 const &origin, 
	unsigned const life)
	: particle(d3d, view), life_(life)
{
	position({ origin });
	scale({ .01f, .01f, .01f });
	translate_right(0.1f);
}

bubble::~bubble()
{
}

void bubble::frame()
{
	--life_;
	transform({ .99f, .99f, .99f });
	translate({
		rand_<float>(-.001f, .001f),
		.001f,
		rand_<float>(-.001f, .001f)
	});
}

bool bubble::alive() const
{
	return life_ != 0U;
}