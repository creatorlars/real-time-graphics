#pragma once

class camera
{
public:
	void frame();
	void render();

	void move(XMFLOAT3 const&);
	void rotate(XMFLOAT3 const&);

	inline void position(XMFLOAT3 const& position) { position_ = position; }
	inline void rotation(XMFLOAT3 const& rotation) { rotation_ = rotation; }

	inline XMFLOAT3 const& position() const { return position_; }
	inline XMFLOAT3 const& rotation() const { return rotation_; }
	inline XMFLOAT4X4 const& matrix() const { return matrix_; }

private:
	XMFLOAT3 position_ = { 0.f, 0.f, 0.f };
	XMFLOAT3 rotation_ = { 0.f, 0.f, 0.f };
	// X+/- == down/up
	// Y+/- == right/left
	// Z+/- == counter-clockwise/clockwise

	XMFLOAT4X4 matrix_;
};