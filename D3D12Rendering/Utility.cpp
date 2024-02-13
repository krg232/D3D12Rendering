#include "utility.h"

std::wstring StrToWstr(std::string input)
{
	auto wch = std::make_unique<wchar_t[]>(input.size() + 1);
	mbstowcs_s(nullptr, wch.get(), input.size() + 1, input.c_str(), _TRUNCATE);
	std::wstring result = wch.get();
	return result;
}

std::string WstrToStr(std::wstring input)
{
	auto ch = std::make_unique<char[]>((input.size() + 1) * 2);
	wcstombs_s(nullptr, ch.get(), (input.size() + 1) * 2, input.c_str(), _TRUNCATE);
	std::string result = ch.get();
	return result;
}

void WriteToLog(HRESULT result, std::string message)
{
	if (result != S_OK)
	{
		_com_error err(result);
		spdlog::error("Failed : "+ message);
		spdlog::error(WstrToStr(err.ErrorMessage()));
		std::exit(1);
	}
	else
	{
		spdlog::info("Succeeded : "+ message);
	}
}
