#pragma once

#include "object.h"

class direct3d;
class light_shader;

class drebbel final : public object
{
public:
	explicit drebbel(direct3d const&, std::shared_ptr<light_shader> const&);

	drebbel() = delete;
	virtual ~drebbel();

	explicit drebbel(drebbel const&) = default;
	explicit drebbel(drebbel&&) = default;

	drebbel& operator=(drebbel const&) = default;
	drebbel& operator=(drebbel&&) = default;

	void frame() override;
	void render(std::shared_ptr<camera> const&) const override;

private:
	direct3d const& d3d_;
	std::shared_ptr<light_shader> shader_ = nullptr;

	class oar final : public object
	{
	public:
		oar(direct3d const&, std::shared_ptr<light_shader> const&);

		oar() = delete;
		virtual ~oar();

		explicit oar(oar const&) = default;
		explicit oar(oar&&) = default;

		oar& operator=(oar const&) = default;
		oar& operator=(oar&&) = default;

		void frame() override;
		void render(std::shared_ptr<camera> const &camera) const override;

	private:
		direct3d const& d3d_;
		std::shared_ptr<light_shader> shader_ = nullptr;
	};
	std::array<std::shared_ptr<oar>, 2U> oars_ = {};
};