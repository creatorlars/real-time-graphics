#pragma once

#include "matrix.h"

class camera : public matrix
{
public:
	camera() = default;
	~camera() = default;

	explicit camera(camera const&) = default;
	explicit camera(camera&&) = default;

	camera& operator=(camera const&) = default;
	camera& operator=(camera&&) = default;

	XMMATRIX const render() const;

	void move(XMFLOAT3);
	inline void move_forward(float const delta)
	{ move({ 0.f, 0.f, delta }); }
	inline void move_backward(float const delta)
	{ move({ 0.f, 0.f, -delta }); }
	inline void move_up(float const delta)
	{ move({ 0.f, delta, 0.f }); }
	inline void move_down(float const delta)
	{ move({ 0.f, -delta, 0.f }); }
	inline void move_left(float const delta)
	{ move({ -delta, 0.f, 0.f }); }
	inline void move_right(float const delta)
	{ move({ delta, 0.f, 0.f }); }
};