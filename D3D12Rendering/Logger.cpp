#include "Logger.h"

Logger::Logger(std::string logFilePath)
{
	_fileLogger = spdlog::basic_logger_mt("D3D12Rendering", logFilePath, true);
	spdlog::set_default_logger(_fileLogger);
	spdlog::info("Start Application");
}

Logger::~Logger()
{
	_fileLogger->flush();
}
