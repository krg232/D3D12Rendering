#pragma once

#include <Windows.h>
#include <memory>
#include "Engine.h"

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

	void InitWindow(const wchar_t* windowName);
};
