#pragma once
#pragma once

#include <string>
#include <memory>
#include <stdexcept>
#include <d3d12.h>
#include <comdef.h>
#include <spdlog/spdlog.h>

std::wstring StrToWstr(std::string input);
std::string WstrToStr(std::wstring input);
void WriteToLog(HRESULT result, std::string message);