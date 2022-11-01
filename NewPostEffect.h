#pragma once
#include "sprite.h"
#include<cassert>

using namespace Microsoft;

class NewPostEffect :
    public Sprite
{
private: // エイリアス
// Microsoft::WRL::を省略
    template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
    // DirectX::を省略
    using XMFLOAT2 = DirectX::XMFLOAT2;
    using XMFLOAT3 = DirectX::XMFLOAT3;
    using XMFLOAT4 = DirectX::XMFLOAT4;
    using XMMATRIX = DirectX::XMMATRIX;

public:

    NewPostEffect();

    void Initilaize();

    void SRVDescHeap();

    void RTVDescHeap();

    void DepthBuff();

    void DSVDescHeap();

    void Draw(ID3D12GraphicsCommandList* cmdList);

    void PreDrawScene(ID3D12GraphicsCommandList* cmdList);

    void PostDrawScene(ID3D12GraphicsCommandList* cmdList);

private:

    ComPtr<ID3D12Resource> texBuff;

    ComPtr<ID3D12DescriptorHeap> descHeapSRV;

    ComPtr<ID3D12Resource> depthBuff;

    ComPtr<ID3D12DescriptorHeap> descHeapRTV;

    ComPtr<ID3D12DescriptorHeap> descHeapDSV;
private:
    static const float clearColor[4];

};

