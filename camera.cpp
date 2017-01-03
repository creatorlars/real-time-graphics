#include "pch.h"
#include "camera.h"

void camera::frame() const
{
}

void camera::render()
{
	auto const position_vec = XMLoadFloat3(&position_);
	auto up_vec = XMLoadFloat3(&up_);
	auto target_vec = XMLoadFloat3(&target_);

	// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in
	// radians.
	auto const pitch = rotation_.x * 0.0174532925f;
	auto const yaw = rotation_.y * 0.0174532925f;
	auto const roll = rotation_.z * 0.0174532925f;

	// Create the rotation matrix from the yaw, pitch, and roll values.
	auto const rotation = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	// Transform the lookAt and up vector by the rotation matrix so the view is
	// correctly rotated at the origin.
	target_vec = XMVector3TransformCoord(target_vec, rotation);
	up_vec = XMVector3TransformCoord(up_vec, rotation);

	// Translate the rotated camera position to the location of the viewer.
	target_vec = XMVectorAdd(position_vec, target_vec);

	// Finally create the view matrix from the three updated vectors.
	auto const matrix = XMMatrixLookAtLH(position_vec, target_vec, up_vec);
	XMStoreFloat4x4(&matrix_, matrix);
}

void camera::move(XMFLOAT3 const& delta)
{
	position_.x += delta.x;
	position_.y += delta.y;
	position_.z += delta.z;
}

void camera::rotate(XMFLOAT3 const& delta)
{
	rotation_.x += delta.x;
	rotation_.y += delta.y;
	rotation_.z += delta.z;
}