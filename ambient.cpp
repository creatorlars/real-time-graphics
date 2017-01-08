#include "pch.h"
#include "ambient.h"

ambient::ambient(XMFLOAT3 const &direction, XMFLOAT4 const &min,
	XMFLOAT4 const &max)
	: direction_(direction), light(min, max)
{}