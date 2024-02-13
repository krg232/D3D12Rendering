#pragma once

#include <string>
#include <memory>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

class Logger
{
public:
	Logger(std::string logFilePath);
	~Logger();

private:
	std::shared_ptr<spdlog::logger> _fileLogger;
};
