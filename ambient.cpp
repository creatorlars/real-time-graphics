#include "pch.h"
#include "ambient.h"

ambient::ambient(XMFLOAT3 const &direction, XMFLOAT3 const &min,
	XMFLOAT3 const &max)
	: direction_(direction), min_(min), max_(max)
{}