#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include<string>

using namespace std;
using namespace DirectX;
using namespace Microsoft::WRL;

class TextureModel
{
private: // エイリアス
// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
public: // サブクラス
// 頂点データ構造体
	struct VertexPosNormalUv
	{
		XMFLOAT3 pos;
		XMFLOAT3 noramal;
		XMFLOAT2 uv;
	};

	// 定数バッファ用データ構造体
	struct ConstBufferData
	{
		XMFLOAT4 color;	// 色 (RGBA)
		XMMATRIX mat;	// ３Ｄ変換行列
	};

public://ファイル読み込み
	static TextureModel* loadFromTex(const wchar_t* pngname);
		static void setDev(ID3D12Device* dev) { TextureModel::dev = dev; }
private: // 定数
	static const int division = 50;					// 分割数
	static const float radius;				// 底面の半径
	static const float prizmHeight;			// 柱の高さ
	static const int planeCount = division * 2 + division * 2;		// 面の数
	static const int vertexCount = 4;		// 頂点数
	static const int indexCount = 3 * 2;
private:
	// デバイス
	static ID3D12Device* dev;
	// デスクリプタサイズ
	static UINT descriptorHandleIncrementSize;
	// コマンドリスト
	static ID3D12GraphicsCommandList* cmdList;
	// ルートシグネチャ
	static ComPtr<ID3D12RootSignature> rootsignature;
	// パイプラインステートオブジェクト
	static ComPtr<ID3D12PipelineState> pipelinestate;
	// デスクリプタヒープ
	static ComPtr<ID3D12DescriptorHeap> descHeap;
	// 頂点バッファ
	static ComPtr<ID3D12Resource> vertBuff;
	// インデックスバッファ
	static ComPtr<ID3D12Resource> indexBuff;
	// テクスチャバッファ
	static ComPtr<ID3D12Resource> texbuff;
	// シェーダリソースビューのハンドル(CPU)
	static CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
	// シェーダリソースビューのハンドル(CPU)
	static CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;
	// ビュー行列
	static XMMATRIX matView;
	// 射影行列
	static XMMATRIX matProjection;
	// 視点座標
	static XMFLOAT3 eye;
	// 注視点座標
	static XMFLOAT3 target;
	// 上方向ベクトル
	static XMFLOAT3 up;
	// 頂点バッファビュー
	static D3D12_VERTEX_BUFFER_VIEW vbView;
	// インデックスバッファビュー
	static D3D12_INDEX_BUFFER_VIEW ibView;
	// 頂点データ配列
	static VertexPosNormalUv vertices[vertexCount];
	// 頂点インデックス配列
	static unsigned short indices[indexCount];

	static XMMATRIX matBillbord;

	static XMMATRIX matBillbordY;
private://非公開のメンバ関数
	void LoadFromTexInternal(const wchar_t* pngname);
public://メンバ関数
	bool LoadTexture(const wchar_t* pngname);
	//デスクリプタヒープの初期化
	bool InitializeDescriptorHeap();
	// モデル作成
	static void CreateModel();
	/// 描画
	void Draw(ID3D12GraphicsCommandList* cmdList);
};

