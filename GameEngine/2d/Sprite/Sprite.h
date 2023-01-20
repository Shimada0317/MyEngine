#pragma once
#include<Windows.h>
#include<wrl.h>
#include<d3d12.h>
#include<d3dx12.h>
#include<DirectXMath.h>

class Sprite
{
private: // エイリアス
// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public://サブクラス

	struct VertexPosUv
	{
		XMFLOAT3 pos;
		XMFLOAT2 uv;
	};

	struct ConstBufferData
	{
		XMFLOAT4 color;//色
		XMMATRIX mat;//3D変換行列
	};

public://静的メンバ関数

	static bool StaticInitialize(ID3D12Device* device, int window_width, int window_height);

	static bool LoadTexture(UINT texNumber, const wchar_t* filename);

	static void PreDraw(ID3D12GraphicsCommandList* cmdList);

	static void PostDraw();

	static Sprite* SpriteCreate(UINT texNumber, XMFLOAT2 position, XMFLOAT4 color = { 1, 1, 1, 1 }, XMFLOAT2 anchorpoint = { 0.0f, 0.0f }, bool isFlipX = false, bool isFlipY = false);

protected://静的メンバ変数
	static const int spriteSRVCount = 512;
	//頂点数
	static const int vertNum = 4;
	//デバイス
	static ID3D12Device* dev;
	// デスクリプタサイズ
	static UINT descriptorHandleIncrementSize;
	//コマンドリスト
	static ID3D12GraphicsCommandList* cmdList;
	//パイプライン
	static ComPtr<ID3D12PipelineState>pipelinestate;
	//ルートシグネチャー
	static ComPtr<ID3D12RootSignature>rootsignature;
	//射影行列
	static XMMATRIX matProjection;
	//テクスチャ用デスクリプタヒープの生成
	static ComPtr<ID3D12DescriptorHeap> descHeap;
	//テクスチャリソース(テクスチャ)の配列
	static ComPtr<ID3D12Resource>texBuff[spriteSRVCount];
	//	//定数バッファ用のデスクリプタヒープ
	static ComPtr<ID3D12DescriptorHeap> basicDescHeap;

public://メンバ関数

	Sprite(UINT texnumber,XMFLOAT2 position, XMFLOAT2 size, XMFLOAT4 color, XMFLOAT2 anchorpoint, bool FlipX, bool FlipY);

	//初期化
	bool InitializeSprite();
	//bool SpriteCreateGraaphicsPipeline(ID3D12Device* dev);
	//回転
	void SetRotation(float rotation);
	//座標
	void SetPosition(XMFLOAT2 position);
	//サイズ
	void SetSize(XMFLOAT2 size);
	//アンカーポイント
	void SetAnchor(XMFLOAT2 anchorpoint);
	//左右反転
	void SetFlipX(bool FlipX);
	//上下反転
	void SetFlipY(bool FlipY);
	//テクスチャ
	void SetTexRect(XMFLOAT2 texBase, XMFLOAT2 texSize);
	//描画
	void Draw();

protected://メンバ変数
		//頂点バッファ
	ComPtr<ID3D12Resource> vertBuff ;
	//定数バッファ
	ComPtr<ID3D12Resource>constBuff ;
	//頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	
	//テクスチャ番号
	UINT texNumber = 0;
	
	//Z軸回りの回転角
	float rotation = 0.0f;
	//座標
	XMFLOAT2 position{0.0f,0.0f};
	//大きさ
	XMFLOAT2 size = { 1.0f,1.0f };
	//ワールド行列
	XMMATRIX matWorld{};
	//色(RGBA)
	XMFLOAT4 color = { 1,1,1,1 };
	//左右上下反転
	bool FlipX = false;
	bool FlipY = false;
	//アンカーポイント
	XMFLOAT2 anchorpoint = { 0,0 };
	//テクスチャ始点
	XMFLOAT2 texBase = { 0,0 };
	//テクスチャ幅
	XMFLOAT2 texSize = { 1.0f,1.0f };
private:
	void Trans();
};

