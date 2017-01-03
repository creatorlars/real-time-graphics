#pragma once

class camera
{
public:
	camera() = default;
	~camera() = default;

	explicit camera(camera const&) = default;
	explicit camera(camera&&) = default;

	camera& operator=(camera const&) = default;
	camera& operator=(camera&&) = default;

	void frame() const;
	void render();

	void move(XMFLOAT3 const&);
	void rotate(XMFLOAT3 const&);

	inline void position(XMFLOAT3 const& position) { position_ = position; }
	inline void rotation(XMFLOAT3 const& rotation) { rotation_ = rotation; }
	inline void matrix(XMFLOAT4X4 const& matrix) { matrix_ = matrix; }

	inline XMFLOAT3 const& position() const { return position_; }
	inline XMFLOAT3 const& rotation() const { return rotation_; }
	inline XMFLOAT4X4 const& matrix() const { return matrix_; }

private:
	XMFLOAT3 up_ = { 0.f, 1.f, 0.f };
	XMFLOAT3 target_ = { 0.f, 0.f, 1.f };
	XMFLOAT3 position_ = {};
	XMFLOAT3 rotation_ = {};
	XMFLOAT4X4 matrix_ = {};
};