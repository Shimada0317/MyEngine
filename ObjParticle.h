#pragma once
#include"Object3d.h"
#include"ObjModel.h"
#include<DirectXMath.h>

const int MAX = 24;

class ObjParticle
{
private: // �G�C���A�X
// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;
public:
	void InitializeState(int i);

	void Initialize();

	void Set(XMVECTOR& enemyPos,bool arive);

	void Update();

	void Draw();

	void Finalize();
private:
	ObjModel* model = nullptr;
	std::unique_ptr<Object3d> particle[MAX];

	XMVECTOR position[MAX];
	XMFLOAT3 rotation = { 0,0,0 };
	XMFLOAT3 scale[MAX];
	XMFLOAT3 up[MAX];
	XMVECTOR go = { 0.01f,0.01f,0.01f };
	bool effect[MAX];
	float numX = 0;
	float numY = 0;
	float scalenumX = 0;
	float scalenumY = 0;
};

