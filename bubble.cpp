#include "pch.h"
#include "bubble.h"

#include "camera.h"

bubble::bubble(direct3d const &d3d, std::shared_ptr<camera> const &camera,
	ComPtr<ID3D11ShaderResourceView> const &view, XMFLOAT3 const &origin, 
	unsigned const life)
	: particle(d3d, camera, view), life_(life)
{
	position({ origin });
	scale({ .01f, .01f, .01f });
}

void bubble::frame()
{
	--life_;
	transform({ .99f, .99f, .99f });
	translate({ 0.f, .001f, 0.f });
}