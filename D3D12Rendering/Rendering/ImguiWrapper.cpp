#include "ImguiWrapper.h"

ImGuiWrapper::~ImGuiWrapper()
{
    // Cleanup
    ImGui_ImplDX12_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

void ImGuiWrapper::InitImgui(HWND hwnd, ID3D12Device *device)
{
    auto frameBuffCount = 2; // �t���[���o�b�t�@�̐�

    // �f�B�X�N���v�^�q�[�v�̍쐬
    _imguiHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    _imguiHeapDesc.NumDescriptors = frameBuffCount;
    _imguiHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    _result = device->CreateDescriptorHeap(&_imguiHeapDesc, IID_PPV_ARGS(_imguiDescHeap.GetAddressOf()));

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    // ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(hwnd);

    ImGui_ImplDX12_Init(device, frameBuffCount, DXGI_FORMAT_R8G8B8A8_UNORM, _imguiDescHeap.Get(), _imguiDescHeap.Get()->GetCPUDescriptorHandleForHeapStart(), _imguiDescHeap.Get()->GetGPUDescriptorHandleForHeapStart());
}

void ImGuiWrapper::SetWindow()
{
    // Start the Dear ImGui frame
    ImGui_ImplDX12_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("info");

    auto framerate = ImGui::GetIO().Framerate;
    ImGui::Text("Framerate:%.3f ms", 1000.0f / framerate);
    ImGui::Text("Mouse Position: (%.1f, %.1f)", ImGui::GetMousePos().x, ImGui::GetMousePos().y);
    ImGui::Text("Mouse Crick?: %s", ImGui::IsMouseClicked(ImGuiMouseButton_Left) ? "true" : "false");

    ImGui::End();

    ImGui::Render();
}

void ImGuiWrapper::Draw(ID3D12GraphicsCommandList *commandList)
{
    ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), commandList);
}

ID3D12DescriptorHeap *const *ImGuiWrapper::GetImguiDescHeapPtr() const
{
    return _imguiDescHeap.GetAddressOf();
}

void ImGuiWrapper::UpdateCameraRotationInput(Camera *camera)
{
    auto &io = ImGui::GetIO();

    // ImGui�E�B���h�E���}�E�X���L���v�`�����Ă���ꍇ�͉������Ȃ�
    if (io.WantCaptureMouse)
    {
        _isDragging = false;
        return;
    }

    auto currentMousePos = io.MousePos;

    // ���N���b�N�J�n
    if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
    {
        _isDragging = true;
        _lastMousePos = currentMousePos;
    }
    // ���N���b�N�I��
    else if (ImGui::IsMouseReleased(ImGuiMouseButton_Left))
    {
        _isDragging = false;
    }

    // �h���b�O��
    if (_isDragging)
    {
        auto delta = ImVec2(currentMousePos.x - _lastMousePos.x, currentMousePos.y - _lastMousePos.y);

        // �}�E�X�������Ă��邩
        if (delta.x != 0.0f || delta.y != 0.0f)
        {
            camera->RotateCamera(delta.x, delta.y);
        }

        // ���݂̃}�E�X�ʒu���X�V
        _lastMousePos = currentMousePos;
    }
}