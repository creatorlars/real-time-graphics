#include "pch.h"
#include "particle_emitter.h"

#include "transparent_shader.h"

particle_emitter::particle_emitter(direct3d const &d3d)
	: texture_(d3d, "data/bubble.tga"), d3d_(d3d)
{
}

void particle_emitter::frame()
{
	std::vector<std::vector<std::shared_ptr<bubble>>::iterator> dead;
	for (auto it = particles_.begin(); it != particles_.end(); ++it)
	{
		auto const particle = (*it);
		if (particle->alive())
		{
			particle->frame();
		}
		else
		{
			dead.emplace_back(it);
		}
	}

	for (auto it = dead.crbegin(); it != dead.crend(); ++it)
	{
		particles_.erase(*it);
	}
}

void particle_emitter::render(std::shared_ptr<transparent_shader> const &shader,
	std::shared_ptr<camera> const &camera)
{
	for (auto const &particle : particles_)
	{
		if (particle->alive())
		{
			shader->render(particle, camera, .5f);
		}
	}
}

void particle_emitter::emit()
{
	particles_.emplace_back(std::make_shared<bubble>(d3d_, texture_, position_, 1'000U));
}

bubble::bubble(direct3d const &d3d, texture const &texture,
	XMFLOAT3 const &position, unsigned const life)
	: quad_(d3d), texture_(texture), position_(position), life_(life)
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