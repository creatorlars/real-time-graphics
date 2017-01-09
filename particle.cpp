#include "pch.h"
#include "particle.h"

#include "camera.h"

particle::particle(direct3d const &d3d,
	ComPtr<ID3D11ShaderResourceView> const &view)
	: view_(view), quad_(std::make_shared<quad>(d3d))
{}

particle::~particle()
{}