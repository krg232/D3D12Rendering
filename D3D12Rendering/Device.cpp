#include "Device.h"

Device::Device()
{
}

void Device::InitDevice()
{
    // フィーチャーレベル一覧
    const D3D_FEATURE_LEVEL levels[]{
        D3D_FEATURE_LEVEL_12_1,
        D3D_FEATURE_LEVEL_12_0,
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
    };

#ifdef DEBUG
    _result = CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(_dxgiFactory.GetAddressOf()));
#else
    _result = CreateDXGIFactory1(IID_PPV_ARGS(_dxgiFactory.GetAddressOf()));
#endif

    // アダプターの列挙と探索
    for (UINT i = 0; _dxgiFactory->EnumAdapters(i, _adapter.GetAddressOf()) != DXGI_ERROR_NOT_FOUND; i++)
    {
        _availableAdapters.push_back(_adapter);
        _adapter.Reset();
    }

    for (auto a : _availableAdapters)
    {
        DXGI_ADAPTER_DESC aDesc = {};
        a->GetDesc(&aDesc);
        auto strDesc = aDesc.Description;
    }

    // アダプターを自動選択にする
    _adapter.Reset();

    for (auto lv : levels)
    {
        if (D3D12CreateDevice(_adapter.Get(), lv, IID_PPV_ARGS(_dev.GetAddressOf())) == S_OK)
        {
            _featureLevel = lv;
            break;
        }
    }

    if (_dev == nullptr)
    {
        return;
    }
}

void Device::InitViewPort(int width, int height)
{
    _viewport.Width = static_cast<float>(width);
    _viewport.Height = static_cast<float>(height);
    _viewport.TopLeftX = 0;
    _viewport.TopLeftY = 0;
    _viewport.MaxDepth = 1.0f;
    _viewport.MinDepth = 0.0f;

    _scissorRect.top = 0;
    _scissorRect.bottom = _scissorRect.top + height;
    _scissorRect.left = 0;
    _scissorRect.right = _scissorRect.left + width;
}

ID3D12Device *Device::GetDevice() const
{
    return _dev.Get();
}

IDXGIFactory6 *Device::GetDxgiFactory() const
{
    return _dxgiFactory.Get();
}

const D3D12_VIEWPORT *Device::GetViewPort() const
{
    return &_viewport;
}

const D3D12_RECT *Device::GetScissorRect() const
{
    return &_scissorRect;
}
