#include "pch.h"
#include "object.h"

#include "direct3d.h"

object::object(direct3d const& d3d, char const *const model_file,
	char const *const texture_file)
	: model_(std::make_shared<::model>(d3d, model_file, texture_file))
{}