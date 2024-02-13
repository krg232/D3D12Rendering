#pragma once

#include <Windows.h>
#include <memory>
#include "Rendering\Engine.h"
#include "Logger.h"

class App
{
public:
	App();
	void Run();

private:

	void Init();
	void Update();

	int _windowWidth;
	int _windowHeight;
	WNDCLASSEX _windowClass;
	HWND _hwnd;
	RECT _wrc;
	MSG _msg;
	std::unique_ptr<Engine> _renderingEngine;
	std::unique_ptr<Logger> _logger;

	void InitWindow(const wchar_t* windowName);
};
