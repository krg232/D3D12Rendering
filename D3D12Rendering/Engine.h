#pragma once

#include <vector>
#include <memory>
#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include "Device.h"
#include "Commands.h"
#include "SwapChain.h"
#include "Fence.h"
#include "Shader.h"
#include "Model.h"
#include"GPUBuffer.h"
#include "Texture.h"
#include "RootSignature.h"
#include "PipelineState.h"

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "DirectXTex.lib")

class Engine
{
public:
	Engine(HWND hwnd);
	void Init();
	void Update();

private:
	const unsigned int WindowWidth = 1920;
	const unsigned int WindowHeight = 1080;

	HWND _hwnd;
	HRESULT _result;

	Microsoft::WRL::ComPtr<ID3D12Device> _dev;
	std::unique_ptr<Device> _device;
	std::unique_ptr<Commands> _commands;
	std::unique_ptr<SwapChain> _swapchain;
	std::unique_ptr<Fence> _fence;
	std::unique_ptr<Shader> _shader;
	std::unique_ptr<Model> _model;
	std::unique_ptr<GPUBuffer> _gpuBuffer;
	std::unique_ptr<Texture> _texture;
	std::unique_ptr<RootSignature> _rootSignature;
	std::unique_ptr<PipelineState> _pipelineState;
};
