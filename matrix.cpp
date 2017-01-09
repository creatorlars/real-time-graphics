#include "pch.h"
#include "matrix.h"

void matrix::rotate(XMFLOAT3 const &delta)
{
	rotation_.x += delta.x;
	rotation_.y += delta.y;
	rotation_.z += delta.z;
}

void matrix::translate(XMFLOAT3 const &delta)
{
	position_.x += delta.x;
	position_.y += delta.y;
	position_.z += delta.z;
}

void matrix::transform(XMFLOAT3 const &delta)
{
	scale_.x *= delta.x;
	scale_.y *= delta.y;
	scale_.z *= delta.z;
}