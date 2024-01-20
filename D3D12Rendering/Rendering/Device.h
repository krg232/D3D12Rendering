#pragma once

#include <vector>
#include <memory>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>

class Device
{
public:
	Device();
	void InitDevice();
	void InitViewPort(int width, int height); // ビューポイントとスクリーン領域の設定
	ID3D12Device *GetDevice() const;
	IDXGIFactory6 *GetDxgiFactory() const;
	const D3D12_VIEWPORT *GetViewPort() const;
	const D3D12_RECT *GetScissorRect() const;

private:
	HRESULT _result;
	Microsoft::WRL::ComPtr<ID3D12Device> _dev;
	Microsoft::WRL::ComPtr<IDXGIFactory6> _dxgiFactory;
	D3D_FEATURE_LEVEL _featureLevel = {};
	std::vector<Microsoft::WRL::ComPtr<IDXGIAdapter>> _availableAdapters;
	Microsoft::WRL::ComPtr<IDXGIAdapter> _adapter;

	D3D12_VIEWPORT _viewport = {};
	D3D12_RECT _scissorRect = {};
};
