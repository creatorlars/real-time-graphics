#include "pch.h"
#include "camera.h"

void camera::render()
{
	// convert to radians and create rotation matrix
	auto const pitch = rotation_.x * .0174532925f;
	auto const yaw = rotation_.y * .0174532925f;
	auto const roll = rotation_.z * .0174532925f;
	auto const rotation = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	// transform the target and up vectors by the rotation matrix
	auto target_vec = XMLoadFloat3(&target_);
	target_vec = XMVector3TransformCoord(target_vec, rotation);
	auto up_vec = XMLoadFloat3(&up_);
	up_vec = XMVector3TransformCoord(up_vec, rotation);

	// translate the rotated camera position to the location of the camera
	auto const position_vec = XMLoadFloat3(&position_);
	target_vec = XMVectorAdd(position_vec, target_vec);

	// create the view matrix
	auto const matrix = XMMatrixLookAtLH(position_vec, target_vec, up_vec);
	XMStoreFloat4x4(&matrix_, matrix);
}

void camera::move(XMFLOAT3 delta)
{
	// convert to radians and create rotation matrix
	auto const pitch = rotation_.x * .0174532925f;
	auto const yaw = rotation_.y * .0174532925f;
	auto const roll = rotation_.z * .0174532925f;
	auto const matrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	// transform movement vector by rotation matrix
	auto vec = XMLoadFloat3(&delta);
	vec = XMVector3TransformCoord(vec, matrix);
	XMStoreFloat3(&delta, vec);

	// update position
	position_.x += delta.x;
	position_.y += delta.y;
	position_.z += delta.z;
}