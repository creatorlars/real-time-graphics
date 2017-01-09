#pragma once

class config
{
public:
	explicit config(std::wstring const&);

	config() = delete;
	~config() = default;

	explicit config(config const&) = default;
	explicit config(config&&) = default;

	config& operator=(config const&) = default;
	config& operator=(config&&) = default;

	std::wstring read(std::wstring const&, std::wstring const&) const;
	template <typename T>
	T read(std::wstring const& app, std::wstring const& key) const
	{
		T value;
		std::wistringstream{ read(app, key) } >> std::boolalpha >> value;
		return value;
	}

	void write(std::wstring const&, std::wstring const&, std::wstring const&) const;
	void write(std::wstring const& app, std::wstring const& key, bool const value) const
	{ write(app, key, std::wstring{ value ? L"true" : L"false" }); }
	//template <typename T>
	//void write(std::wstring const& app, std::wstring const& key, T const& value) const
	//{ write(app, key, std::to_wstring(value)); }

private:
	std::wstring filename_ = {};
};