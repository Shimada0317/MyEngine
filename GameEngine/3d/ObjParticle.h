#pragma once
#include"Object3d.h"
#include"ObjModel.h"
#include"ModelManager.h"
#include<DirectXMath.h>
#include"Texture.h"

const int MAX = 20;

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

	void InitializeState(int i);

	void Initialize();

	void Set(XMVECTOR& enemyPos,XMFLOAT3& allRot);

	void Updata(XMVECTOR& enemyPos,XMFLOAT3& allRot);

	void Effect();

	void Draw();

	void Finalize();

	bool IsDelete()const { return Delete_; }
private:

	std::unique_ptr<Object3d>Worm[MAX];
	std::unique_ptr<Object3d> Gear[MAX];
	//ギアのステータス
	XMVECTOR Gear_Pos[MAX];
	XMFLOAT3 Gear_Rot = { 90,0,0 };
	XMFLOAT3 Gear_Scl[MAX];
	//芯のステータス
	XMVECTOR Worm_Pos[MAX];
	XMFLOAT3 Worm_Rot = { 90.0f,0.0f,0.0f };
	XMFLOAT3 Worm_Scl[MAX];

	XMFLOAT3 Up[MAX];

	bool Effect_F[MAX];
	float Gear_NumX[MAX];
	float Gear_NumY[MAX];
	float Gear_NumZ[MAX];
	float Scale_NumX = 0;
	float Scale_NumY = 0;
	float Worm_NumX[MAX];
	float Worm_NumY[MAX];
	float Worm_NumZ[MAX];

	bool Delete_ = false;
	float Gravity[MAX];
};

