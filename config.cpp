#include "pch.h"
#include "config.h"

config::config(std::wstring const &filename)
	: filename_(filename)
{}

std::wstring config::read(std::wstring const &app, std::wstring const& key) const
{
	auto out = std::array<TCHAR, 64U>{};
	GetPrivateProfileString(app.c_str(), key.c_str(), L"0", out.data(),
		static_cast<DWORD>(out.size()), filename_.c_str());
	return { out.data() };
}

void config::write(std::wstring const &app, std::wstring const &key,
	std::wstring const &value) const
{
	WritePrivateProfileString(app.c_str(), key.c_str(), value.c_str(),
		filename_.c_str());
}