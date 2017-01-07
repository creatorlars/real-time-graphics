#include "pch.h"
#include "bubble.h"
#include "camera.h"

bubble::bubble(direct3d const &d3d, texture const &texture,
	XMFLOAT3 const &position, std::shared_ptr<camera> const &camera,
	unsigned const life)
	: quad_(d3d), texture_(texture), position_(position), camera_(camera),
	life_(life)
{
	auto const xmmatrix = XMMatrixIdentity();
	XMStoreFloat4x4(&matrix_, xmmatrix);
}

void bubble::frame()
{
	--life_;
	scale_.x *= 0.99f;
	scale_.y *= 0.99f;
	scale_.z *= 0.99f;
	position_.y += .001f;
}

void bubble::render()
{
	auto xmmatrix = XMMatrixIdentity();

	auto const angle = std::atan2(position_.x - camera_->position().x, position_.z - camera_->position().z) * (180.0 / XM_PI);
	xmmatrix *= XMMatrixRotationY(XMConvertToRadians(static_cast<float>(angle)));
	xmmatrix *= XMMatrixScaling(scale_.x, scale_.y, scale_.z);
	xmmatrix *= XMMatrixTranslation(position_.x, position_.y, position_.z);
	
	XMStoreFloat4x4(&matrix_, xmmatrix);

	quad_.render();
}

void bubble::move(XMFLOAT3 const &delta)
{
	position_.x += delta.x;
	position_.y += delta.y;
	position_.z += delta.z;
}