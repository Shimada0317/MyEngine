#pragma once
#include"Object3d.h"
#include"ObjModel.h"
#include<DirectXMath.h>

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
	void Initialize();

	void Set(XMVECTOR& enemyPos);

	void Update();

	void Draw();

	void Finalize();
private:
	ObjModel* model = nullptr;
	std::unique_ptr<Object3d> particle[10];

	XMVECTOR position = { 0,0,0 };
	XMFLOAT3 rotation = { 0,0,0 };
	XMFLOAT3 scale = { 1.1,1.1,1.1 };
};

