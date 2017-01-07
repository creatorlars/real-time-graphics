#include "pch.h"
#include "camera.h"

XMMATRIX const camera::render() const
{
	// convert to radians and create rotation matrix
	auto const r = rotation();
	auto const pitch = XMConvertToRadians(r.x);
	auto const yaw = XMConvertToRadians(r.y);
	auto const roll = XMConvertToRadians(r.z);
	auto const rotation = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	// transform the target and up vectors by the rotation matrix
	auto constexpr up = XMFLOAT3{ 0.f, 1.f, 0.f };
	auto constexpr target = XMFLOAT3{ 0.f, 0.f, 1.f };
	auto target_vec = XMLoadFloat3(&target);
	target_vec = XMVector3TransformCoord(target_vec, rotation);
	auto up_vec = XMLoadFloat3(&up);
	up_vec = XMVector3TransformCoord(up_vec, rotation);

	// translate the rotated camera position to the location of the camera
	auto const position_vec = XMLoadFloat3(&position());
	target_vec = XMVectorAdd(position_vec, target_vec);

	// return the view matrix
	return XMMatrixLookAtLH(position_vec, target_vec, up_vec);
}

void camera::move(XMFLOAT3 delta)
{
	// convert to radians and create rotation matrix
	auto const r = rotation();
	auto const pitch = XMConvertToRadians(r.x);
	auto const yaw = XMConvertToRadians(r.y);
	auto const roll = XMConvertToRadians(r.z);
	auto const matrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	// transform movement vector by rotation matrix
	auto vec = XMLoadFloat3(&delta);
	vec = XMVector3TransformCoord(vec, matrix);
	XMStoreFloat3(&delta, vec);

	// update position
	translate(delta);
}