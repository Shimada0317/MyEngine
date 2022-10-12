#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>

#include <string>

#include "ObjModel.h"
#include "Camera.h"
#include "LightGroup.h"
#include<memory>


/// <summary>
/// 3Dオブジェクト
/// </summary>
class Object3d
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;
public: // サブクラス

	// パイプラインセット
	struct PipelineSet
	{
		// ルートシグネチャ
		ComPtr<ID3D12RootSignature> rootsignature;
		// パイプラインステートオブジェクト
		ComPtr<ID3D12PipelineState> pipelinestate;
	};

	// 定数バッファ用データ構造体B0
	struct ConstBufferDataB0
	{

		//XMFLOAT4 color;
		XMMATRIX viewproj;    // ビュープロジェクション行列
		XMMATRIX world; // ワールド行列
		XMFLOAT3 cameraPos; // カメラ座標（ワールド座標）

	};

public: // 静的メンバ関数
	/// <summary>
	/// 静的初期化
	/// </summary>
	/// <param name="device">デバイス</param>
	static void StaticInitialize(ID3D12Device* device, Camera* camera = nullptr);

	

	/// <summary>
	/// カメラのセット
	/// </summary>
	/// <param name="camera">カメラ</param>
	static void SetCamera(Camera* camera) {
		Object3d::camera = camera;
	}

	/// <summary>
	/// ライトグループのセット
	/// </summary>
	/// <param name="lightGroup">ライトグループ</param>
	static void SetLightGroup(LightGroup* lightGroup) {
		Object3d::lightGroup = lightGroup;
	}

	/// <summary>
	/// 描画前処理
	/// </summary>
	/// <param name="cmdList">描画コマンドリスト</param>
	static void PreDraw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// 描画後処理
	/// </summary>
	static void PostDraw();

	/// <summary>
	/// 3Dオブジェクト生成
	/// </summary>
	/// <returns></returns>
	static std::unique_ptr<Object3d> Create(ObjModel* model = nullptr);

	/// <summary>
	/// グラフィックパイプラインの生成
	/// </summary>
	static void CreateGraphicsPipeline();

private: // 静的メンバ変数
	// デバイス
	static ID3D12Device* device;
	// コマンドリスト
	static ID3D12GraphicsCommandList* cmdList;
	// パイプライン
	static PipelineSet pipelineSet;
	// カメラ
	static Camera* camera;
	// ライト
	static LightGroup* lightGroup;

public: // メンバ関数
	
	~Object3d();
	
	bool Initialize();
	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update(const XMVECTOR velocity={0,0,0});

	

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>

	/// 座標の取得
	/// </summary>
	/// <returns>座標</returns>
	const XMVECTOR& GetPosition() { return position; }

	/// <summary>
	/// 回転の取得

	/// モデル作成

	/// </summary>
	/// <returns>回転</returns>
	const XMFLOAT3& GetRotation() { return rotation; }

	/// <summary>
	/// 座標の設定
	/// </summary>
	/// <param name="position">座標</param>
	void SetPosition(const XMVECTOR& position) { this->position = position; }

	void SetRotation(const XMFLOAT3& rotation) { this->rotation = rotation; }

	void SetMatrix(const XMMATRIX& mat) { this->matWorld = mat; }

	const XMMATRIX& GetMatrix() { return matWorld; }

	void SetMatrixR(const XMMATRIX& matrot) { this->matRot = matrot; }

	const XMMATRIX& GetMatrixR() { return matRot; }
	/// <summary>
	/// スケールの設定
	/// </summary>
	/// <param name="position">スケール</param>
	void SetScale(const XMFLOAT3& scale) { this->scale = scale; }

	/// <summary>
	/// モデルのセット
	/// </summary>
	/// <param name="model">モデル</param>
	void SetModel(ObjModel* model) { this->model = model; }

	/// <summary>
	/// ビルボードフラグのセット
	/// </summary>
	/// <param name="isBillboard">ビルボードか</param>
	void SetBillboard(bool isBillboard) { this->isBillboard = isBillboard; }

private: // メンバ変数
	ComPtr<ID3D12Resource> constBuffB0; // 定数バッファ
	// 色
	XMFLOAT4 color = { 1,1,1,1 };
	// ローカルスケール
	XMFLOAT3 scale = { 1,1,1 };
	// X,Y,Z軸回りのローカル回転角
	XMFLOAT3 rotation = { 0,0,0 };
	// ローカル座標
	XMVECTOR position = { 0,0,0 };
	// ローカルワールド変換行列
	XMMATRIX matWorld;
	// 親オブジェクト
	Object3d* parent = nullptr;
	//カメラ親オブジェクト
//	Camera* cameraParent = nullptr;
//	RailCamera* cameraP = nullptr;
	// モデル
	ObjModel* model = nullptr;
	// ビルボード
	bool isBillboard = false;

	XMMATRIX matScale, matRot, matTrans;
};

