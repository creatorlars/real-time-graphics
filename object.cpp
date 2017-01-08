#include "pch.h"
#include "object.h"

#include "direct3d.h"
#include "shader.h"

object::object(direct3d const& d3d, char const *const model_file,
    char const *const texture_file)
	: model_(std::make_shared<::model>(d3d, model_file, texture_file))
{}

XMMATRIX const object::render() const
{
	model_->render();

	auto matrix = XMMatrixIdentity();

	// scale
	auto const s = scale();
	matrix *= XMMatrixScaling(s.x, s.y, s.z);

	// rotate
	auto const r = rotation();
	auto const pitch = XMConvertToRadians(r.x);
	auto const yaw = XMConvertToRadians(r.y);
	auto const roll = XMConvertToRadians(r.z);
	matrix *= XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	// translate
	auto const p = position();
	matrix *= XMMatrixTranslation(p.x, p.y, p.z);

	return matrix;
}