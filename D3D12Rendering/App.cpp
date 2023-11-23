#include "App.h"

App::App() : _windowClass{}, _msg{}
{
}

void App::Run()
{
	this->Init();
	this->Update();
}

void App::Init()
{
	_windowHeight = 1080;
	_windowWidth = 1920;

	InitWindow(L"D3D12Rendering");
	_renderingEngine = std::make_unique<Engine>(_hwnd);
	_renderingEngine->Init();
}

void App::Update()
{
	while (_msg.message != WM_QUIT)
	{
		if (PeekMessage(&_msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&_msg);
			DispatchMessage(&_msg);
		}
		else
		{
			_renderingEngine->Update();
		}
	}
}

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		break;
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void App::InitWindow(const wchar_t* windowName)
{
	_windowClass.cbSize = sizeof(WNDCLASSEX);
	_windowClass.lpfnWndProc = WindowProcedure;
	_windowClass.lpszClassName = windowName;
	_windowClass.hInstance = GetModuleHandle(nullptr);

	RegisterClassEx(&_windowClass);

	_wrc = { 0, 0, _windowWidth, _windowHeight };

	AdjustWindowRect(&_wrc, WS_OVERLAPPEDWINDOW, false);

	_hwnd = CreateWindowEx(
		0,
		_windowClass.lpszClassName,
		windowName,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		_wrc.right - _wrc.left,
		_wrc.bottom - _wrc.top,
		nullptr,
		nullptr,
		_windowClass.hInstance,
		nullptr);

	if (_hwnd == nullptr)
	{
		return;
	}

	ShowWindow(_hwnd, SW_SHOW);
}
