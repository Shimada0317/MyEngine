#pragma once
#include"Object3d.h"
#include"ObjModel.h"
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

	void Set(XMVECTOR& enemyPos);

	void Update();

	void Effect();

	void Draw();

	void Finalize();
private:
	ObjModel* model = nullptr;
	ObjModel* worm;
	std::unique_ptr<Object3d>Worm[MAX];
	std::unique_ptr<Object3d> particle[MAX];
	std::unique_ptr<Texture>par[MAX];

	XMVECTOR position[MAX];
	XMFLOAT3 rotation = { 90,0,0 };
	XMFLOAT3 scale[MAX];

	XMVECTOR Wposition[MAX];
	XMFLOAT3 Wrotation = { 90.0f,0.0f,0.0f };
	XMFLOAT3 wscale[MAX];

	XMFLOAT3 up[MAX];
	XMVECTOR go = { 0.01f,0.01f,0.01f };
	bool effect[MAX];
	float numX[MAX];
	float numY[MAX];
	float scalenumX = 0;
	float scalenumY = 0;
	float wnumX[MAX];
	float wnumY[MAX];

};

