#include "pch.h"
#include "spotlight.h"

spotlight::spotlight(XMFLOAT3 const &position, XMFLOAT4 const &min,
	XMFLOAT4 const &max)
	: position_(position), light(min, max)
{}