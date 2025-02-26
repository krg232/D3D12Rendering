#pragma once

#include "..\imgui\imgui.h"
#include "..\imgui\backends\imgui_impl_win32.h"
#include "..\imgui\backends\imgui_impl_dx12.h"
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include "Camera.h"

class ImGuiWrapper
{
public:
	~ImGuiWrapper();
	void InitImgui(HWND hwnd, ID3D12Device *device);
	void SetWindow();
	void Draw(ID3D12GraphicsCommandList *commandList);
	ID3D12DescriptorHeap *const *GetImguiDescHeapPtr() const;
	void UpdateCameraRotationInput(Camera *camera);

private:
	HRESULT _result;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> _imguiDescHeap;
	D3D12_DESCRIPTOR_HEAP_DESC _imguiHeapDesc = {};

	bool _isDragging = false;
	ImVec2 _lastMousePos = {0.0f, 0.0f};
};
