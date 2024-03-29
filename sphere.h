#pragma once

#include "object.h"

class direct3d;
class transparent_shader;

class sphere final : public object
{
public:
	explicit sphere(direct3d const&, std::shared_ptr<transparent_shader> const&);

	sphere() = delete;
	virtual ~sphere();

	explicit sphere(sphere const&) = default;
	explicit sphere(sphere&&) = default;

	sphere& operator=(sphere const&) = default;
	sphere& operator=(sphere&&) = default;

	void frame() override;
	void render(std::shared_ptr<camera> const&) const override;

private:
	direct3d const &d3d_;
	std::shared_ptr<transparent_shader> shader_ = nullptr;
};