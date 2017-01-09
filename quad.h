#pragma once

class direct3d;

class quad
{
public:
	explicit quad(direct3d const&);

	quad() = delete;
	virtual ~quad();

	explicit quad(quad const&) = default;
	explicit quad(quad&&) = default;

	quad& operator=(quad const&) = default;
	quad& operator=(quad&&) = default;

	void render() const;

	inline unsigned index_count() const
	{ return index_count_; }

private:
	ComPtr<ID3D11Buffer> vertex_buffer_ = nullptr;
	ComPtr<ID3D11Buffer> index_buffer_ = nullptr;
	constexpr static unsigned index_count_ = 6U;

	direct3d const &d3d_;
};