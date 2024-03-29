#pragma once
#include "Sprite.h"
#include<Windows.h>
#include<wrl.h>
#include<d3d12.h>
#include<d3dx12.h>
#include"DirectXCommon.h"

class PostEffect :
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
    struct  ConstD
    {
        XMFLOAT4 col;//色
    };

public:
    ///<summary>
    ///コンストラクタ
    ///</summary>
    PostEffect();

    ~PostEffect();

    ///<summary>
    ///パイプライン生成
    ///</summary>
    void CreatePipeline();

    void CreateVertBuff();

    void CreateConstBuff();

    void CreateTexBuff();

    void SRVDesc();

    void RTVDesc();

    void DepthBuff();

    void DSVDesc();

    ///<summary>
    ///初期化
    ///</summary>
    void Initialize();

    void Update(const XMFLOAT4& col);

    ///<summary>
    ///描画
    ///</summary>
    ///<param name="cmdList">コマンドリスト</param>
    void Draw(ID3D12GraphicsCommandList* cmdList);

    ///<summary>
   ///描画前準備
   ///</summary>
   ///<param name="cmdList">コマンドリスト</param>
    void PreDrawScene(ID3D12GraphicsCommandList* cmdList);

   ///<summary>
   ///描画後処理
   ///</summary>
   ///<param name="cmdList">コマンドリスト</param>
   void PostDrawScene(ID3D12GraphicsCommandList* cmdList);

   void SetCol(const XMFLOAT4& col) { this->color = col; }

   

private:
    //テクスチャバッファ
    ComPtr<ID3D12Resource>texBuff;
    //SRV用のデスクリプタヒープ
    ComPtr<ID3D12DescriptorHeap>descHeapSRV;
    //深度バッファ
    ComPtr<ID3D12Resource> depthBuff;
    //RTV(レンダーターゲット)用デスクリプタヒープ
    ComPtr<ID3D12DescriptorHeap> descHeapRTV;
    //DSV(デプステンシルビュー)用デスクリプタヒープ
    ComPtr<ID3D12DescriptorHeap> descHeapDSV;
    //グラフィックスパイプライン
    ComPtr<ID3D12PipelineState> pipelineState;
    //ルートシグネチャ
    ComPtr<ID3D12RootSignature> rootSignature;

private://静的メンバ変数
    //画面クリアカラー
    static const float clearColor[4];

    XMFLOAT4 color={0.0f,1.0f,1.0f,1.0f};

};

