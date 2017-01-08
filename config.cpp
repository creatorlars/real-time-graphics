#include "pch.h"
#include "config.h"

config::config(std::wstring filename)
	: filename_(filename)
{}

std::wstring config::read(std::wstring const &app, std::wstring const& key) const
{
	TCHAR test[64];
	GetPrivateProfileString(app.c_str(), key.c_str(), L"0", test, 64, filename_.c_str());
	return { test };
}

void config::write(std::wstring const &app, std::wstring const &key,
	std::wstring const &value) const
{
	WritePrivateProfileString(app.c_str(), key.c_str(), value.c_str(), filename_.c_str());
}