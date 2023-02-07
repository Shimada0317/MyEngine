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

	void Initialize(int ModelNumber,const XMVECTOR& particlepos,const XMFLOAT3& particlescl,const XMFLOAT3& particlerot);

	void Set();

	void Update(XMVECTOR& enemyPos,XMFLOAT3& allRot);

	void Draw();

	void Finalize();

	bool IsDelete()const { return Delete_; }
private:

	std::unique_ptr<Object3d>Particle;
	//パーティクルステータス
	XMVECTOR ParticlePos = { 0.0f,0.0f,0.0f };
	XMFLOAT3 ParticleScl = { 0.0f,0.0f,0.0f };
	XMFLOAT3 ParticleRot = { 0.0f,0.0f,0.0f };

	bool Delete_;
	float RandomY;
	float RandomX;
	float SmoleScl;
	float time = 0.0f;
	float gravity = 0.6f;
};

