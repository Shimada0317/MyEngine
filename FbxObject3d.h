#pragma once

#include"FbxModel.h"
#include"Camera.h"

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include<string>

class FbxObject3d
{
public://定数
//ボーンの最大数
	static const int MAX_BONES = 32;
protected:
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
public://サブクラス
	struct ConstBufferDataTransForm
	{
		XMMATRIX viewproj;
		XMMATRIX world;
		XMFLOAT3 camerapos;
	};

	struct ConstBufferDataSkin
	{
		XMMATRIX bones[MAX_BONES];
	};
public://静的メンバ
	static void SetDevice(ID3D12Device* device) { FbxObject3d::device = device; }
	static void SetCamera(Camera* camera) { FbxObject3d::camera = camera; }
public://メンバ関数
/// <summary>
/// 初期化
/// </summary>
	void Initialize();

/// <summary>
/// グラフィックスパイプラインの生成
/// </summary>
	static void CreateGraphicPipeline();
/// <summary>
/// 毎フレーム処理
/// </summary>
	void Update();
	/// <summary>
/// 描画
/// </summary>
	void Draw(ID3D12GraphicsCommandList* cmdList);

/// <summary>
/// モデルのセット
/// </summary>
	void SetModel(FbxModel* model) { this->model = model; }
/// <summary>
/// 角度のセット
/// </summary>
	void SetRotation(XMFLOAT3 rotation) { this->rotation = rotation; }

	/// <summary>
/// 角度のセット
/// </summary>
	void SetPosition(XMFLOAT3 position) { this->position = position; }

/// <summary>
/// アニメーション開始
/// </summary>
	void PlayAnimation();

protected://メンバ変数
	ComPtr<ID3D12Resource>ConstBufferTransForm;

	static ComPtr<ID3D12RootSignature> rootsignature;

	static ComPtr<ID3D12PipelineState> pipelinestate;

	ComPtr<ID3D12Resource> constBuffSkin;
private:
	static ID3D12Device* device;

	static Camera* camera;

protected:
	XMFLOAT3 scale = { 1,1,1 };

	XMFLOAT3 rotation = { 0,0,0 };

	XMFLOAT3 position = { 0,0,0 };

	XMMATRIX matWorld;

	FbxModel* model = nullptr;
public:
	//1フレームの時間
	FbxTime frameTime;
	//アニメーション開始時間
	FbxTime startTime;
	//アニメーション終了時間
	FbxTime endTime;
	//現在時間
	FbxTime currentTime;
	//アニメーション再生中
	bool isPlay = false;
};

