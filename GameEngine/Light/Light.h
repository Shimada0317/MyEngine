#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include<string>

#pragma once
class Light
{
private: // エイリアス
// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
public://サブクラス
	struct ConstBufferData {
		XMVECTOR lightv;//ライトの方向ベクトル
		XMFLOAT3 lightcolor;//ライトの色
	};
private://静的メンバ変数
	//デバイス
	static ID3D12Device* dev;
public://静的メンバ関数

	static void StaticInitialize(ID3D12Device* dev);

	static std::unique_ptr<Light>Create();
private://メンバ変数
	ComPtr<ID3D12Resource> constBuff;
	//ライトの光源方向
	XMVECTOR lightdir = { 1,0,0,0 };
	//ライトの色
	XMFLOAT3 lightcolor = { 1,1,1 };
	//ダーティフラグ
	bool dirty = false;
public://メンバ関数
	void Initialize();

	void TransConstBuffer();

	void SetLightDir(const XMVECTOR& lightdir);

	void SetLightColor(const XMFLOAT3& lightcolor);

	void Update();

	void Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParameterIndex);


};

