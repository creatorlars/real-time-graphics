#include "pch.h"
#include "light.h"

light::light(XMFLOAT4 const &colour)
{
	ambient_ = colour;
	diffuse_ = colour;
}