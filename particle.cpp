#include "pch.h"
#include "particle.h"

#include "camera.h"

particle::particle(direct3d const &d3d, std::shared_ptr<camera> const &camera,
	ComPtr<ID3D11ShaderResourceView> const &view)
	: quad(d3d), camera_(camera), view_(view)
{}

XMMATRIX const particle::render() const
{
	quad::render();

	auto xmmatrix = XMMatrixIdentity();

	// billboard
	auto const p = position();
	auto const x = p.x - camera_->position().x;
	auto const z = p.z - camera_->position().z;
	auto const angle = static_cast<float>(std::atan2(x, z));
	xmmatrix *= XMMatrixRotationY(angle);

	auto const s = scale();
	xmmatrix *= XMMatrixScaling(s.x, s.y, s.z);
	xmmatrix *= XMMatrixTranslation(p.x, p.y, p.z);

	return xmmatrix;
}