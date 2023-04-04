#pragma once
#include"Object3d.h"
#include"ObjModel.h"
#include"ModelManager.h"
#include<DirectXMath.h>
#include"Texture.h"

class ObjParticle
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
public:
	~ObjParticle();

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="ModelNumber">モデルマネージャーの</param>
	/// <param name="particlepos">パーティクル発生地点</param>
	/// <param name="particlescl">パーティクルの初期サイズ</param>
	/// <param name="particlerot">パーティクルの向き</param>
	void Initialize(int ModelNumber,const XMVECTOR& particlepos,const XMFLOAT3& particlescl,const XMFLOAT3& particlerot);
	/// <summary>
	/// ステータスセット
	/// </summary>
	void Set();
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update(int smole=200);
	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();
	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize();

	/// <summary>
	/// 消去処理
	/// </summary>
	/// <returns>消去フラグ</returns>
	bool IsDelete()const { return Delete_; }
private:

	std::unique_ptr<Object3d>Particle;
	//パーティクルステータス
	XMVECTOR ParticlePos = { 0.0f,0.0f,0.0f };
	XMFLOAT3 ParticleScl = { 0.0f,0.0f,0.0f };
	XMFLOAT3 ParticleRot = { 0.0f,0.0f,0.0f };
	//消去フラグ
	bool Delete_;
	//ランダム用変数
	float RandomZ;
	float RandomY;
	float RandomX;
	float RandomRot;
	float SmoleScl;
	//時間
	float time = 0.0f;
	//重力
	float gravity = 2.0f;
};

