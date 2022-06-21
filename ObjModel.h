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

class ObjModel
{
public: // サブクラス
// 頂点データ構造体
	struct VertexPosNormalUv
	{
		DirectX::XMFLOAT3 pos; // xyz座標
		DirectX::XMFLOAT3 normal; // 法線ベクトル
		DirectX::XMFLOAT2 uv;  // uv座標
	};

	struct Material
	{
		std::string name;
		DirectX::XMFLOAT3 ambient;
		DirectX::XMFLOAT3 diffuse;
		DirectX::XMFLOAT3 specular;
		float alpha;
		std::string textureFilename;
		//コンストラクト
		Material() {
			ambient = { 0.3f,0.3f,0.3f };
			diffuse = { 0.0f,0.0f,0.0f };
			specular = { 0.0f,0.0f,0.0f };
			alpha = 1.0f;
		}
	};

	struct ConstBufferDataB1
	{
		DirectX::XMFLOAT3 ambient;//アンビエント
		float pad1;//パディング
		DirectX::XMFLOAT3 diffuse;//ディフューズ
		float pad2;//パディング
		DirectX::XMFLOAT3 specular;//スペキュラー
		float alpha;//アルファ
	};
public:
	//OBJファイル読み込み
	static ObjModel* LoadFromObJ(const std::string& modelname);
	static void SetDev(ID3D12Device* dev) { ObjModel::dev = dev; }
private://メンバ変数
	//インデックス配列
	std::vector<unsigned short> indices;
	// 頂点バッファ
	ID3D12Resource* vertBuff;
	// インデックスバッファ
	ID3D12Resource* indexBuff;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView;
	// インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW ibView;
	// 頂点データ配列
	//static VertexPosNormalUv vertices[vertexCount];
	std::vector<VertexPosNormalUv> vertices;
	//定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource>constBuffB1;
	//マテリアル
	Material material;
	// デスクリプタサイズ
	UINT descriptorHandleIncrementSize;
	// デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeap;
	// シェーダリソースビューのハンドル(CPU)
	CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
	// シェーダリソースビューのハンドル(CPU)
	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;
	// テクスチャバッファ
	ComPtr<ID3D12Resource> texbuff;

private://非公開のメンバ関数
	void LoadFromOBJInternal(const std::string&  modelname);
public://メンバ関数
	//マテリアル読み込み
	void LoadMaterial(const std::string& directoryPath, const std::string& filename);
	//テクスチャ読み込み
	bool LoadTexture(const std::string& directoryPath, const std::string& filename);
	//デスクリプタヒープの初期化
	void InitializeDescriptorHeap();
	//各種バッファ生成
	void CreateBuffers();
	//描画
	void Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParamIndexMaterial);
private://静的メンバ変数
	//デバイス
	static ID3D12Device* dev;
};

