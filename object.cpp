#include "pch.h"
#include "object.h"

#include "direct3d.h"

object::object(direct3d const& d3d, char const *const model_file,
    char const *const texture_file)
	: model_(std::make_shared<::model>(d3d, model_file, texture_file))
{
	auto const xmmatrix = XMMatrixIdentity();
	XMStoreFloat4x4(&matrix_, xmmatrix);
}

void object::render()
{
	auto xmmatrix = XMMatrixIdentity();
	xmmatrix *= XMMatrixScaling(scale_.x, scale_.y, scale_.z);

	auto const pitch = XMConvertToRadians(rotation_.x);
	auto const yaw = XMConvertToRadians(rotation_.y);
	auto const roll = XMConvertToRadians(rotation_.z);
	xmmatrix *= XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	xmmatrix *= XMMatrixTranslation(position_.x, position_.y, position_.z);

	XMStoreFloat4x4(&matrix_, xmmatrix);

	model_->render();
}

void object::rotate(XMFLOAT3 const &delta)
{
	rotation_.x += delta.x;
	rotation_.y += delta.y;
	rotation_.z += delta.z;
}

void object::move(XMFLOAT3 const &delta)
{
	position_.x += delta.x;
	position_.y += delta.y;
	position_.z += delta.z;
}