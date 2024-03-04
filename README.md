# D3D12Renddering

グラフィックスプログラミングとDirectX12レンダリング勉強用のプロジェクト

## 開発環境

**OS:** Windows 11  
**開発環境:** Visual Studio 2022  
**SDK:** Windows SDK  
**利用ライブラリ**

- [DirectXTex](https://github.com/microsoft/DirectXTex)
- [assimp](https://github.com/assimp/assimp)
- [Dear ImGui](https://github.com/ocornut/imgui)
- [spdlog](https://github.com/gabime/spdlog)

## ビルド方法

#### リポジトリのクローン

```bash
git clone https://github.com/krg232/D3D12Rendering.git
```

#### 依存ライブラリのインストール

DirectXTexを`D3D12Rendering/ThirdParty`にクローンしてビルドする

```bash
cd D3D12Rendering
git clone https://github.com/microsoft/DirectXTex.git D3D12Rendering/ThirdParty
```

[vcpkg](https://github.com/microsoft/vcpkg)でassimp, spdlogをインストールする

```bash
vcpkg install assimp
vcpkg install spdlog
```

#### アセット配置

`D3D12Rendering/Assets`にモデルを配置する

#### ビルド

`D3D12Rendering.sln`をVisual Studioで開いてビルドする
