#include "pch.h"
#include "camera.h"

void camera::frame()
{
}

void camera::render()
{
	auto const position_vec = XMLoadFloat3(&position_);

	// Setup the vector that points upwards.
	auto const up = XMFLOAT3{ 0.f, 1.f, 0.f };
	auto up_vec = XMLoadFloat3(&up);

	// Setup where the camera is looking by default.
	auto const look_at = XMFLOAT3{ 0.f, 0.f, 1.f };
	auto look_at_vec = XMLoadFloat3(&look_at);

	// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in
	// radians.
	auto const pitch = rotation_.x * 0.0174532925f;
	auto const yaw = rotation_.y * 0.0174532925f;
	auto const roll = rotation_.z * 0.0174532925f;

	// Create the rotation matrix from the yaw, pitch, and roll values.
	auto const rotation = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	// Transform the lookAt and up vector by the rotation matrix so the view is
	// correctly rotated at the origin.
	look_at_vec = XMVector3TransformCoord(look_at_vec, rotation);
	up_vec = XMVector3TransformCoord(up_vec, rotation);

	// Translate the rotated camera position to the location of the viewer.
	look_at_vec = XMVectorAdd(position_vec, look_at_vec);

	// Finally create the view matrix from the three updated vectors.
	auto const matrix = XMMatrixLookAtLH(position_vec, look_at_vec, up_vec);
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