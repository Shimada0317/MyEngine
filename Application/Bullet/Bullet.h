#pragma once
#include<DirectXMath.h>
#include"Object3d.h"
#include"ObjModel.h"
#include"Action.h"
#include"Collision.h"
#include <d3dx12.h>

#include<memory>

using namespace DirectX;
using namespace Microsoft::WRL;
using namespace std;

class Player;
class Bullet
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;
public:
	~Bullet();

	void Initialize();

	void Test(const XMVECTOR& GWorldPos, const XMVECTOR& PWorldPos, const XMFLOAT3& Eye_Rot,const XMFLOAT3& rot);

	void Set();

	bool CheckOk();

	void TriggerOn();

	void Update();

	void Draw();

	void ImgUiDraw();

	void Finalize();

#pragma region Get
	// 座標のゲット
	const XMVECTOR& GetPosition() { return pos; }
	//角度のゲット
	const XMFLOAT3& GetRotation() { return rot; }
	//スケールのゲット
	const XMFLOAT3& GetScl() { return scl; }
	//発射フラグのゲット
	bool GetTrigger() { return Trigger; }
#pragma endregion

#pragma region Set
	//座標のセット
	void SetPosition(XMVECTOR position) { this->pos = position; }
	//角度のセット
	void SetRotation(XMFLOAT3 rotation) { this->rot = rotation; }
	//スケールのセット
	void SetScl(const XMFLOAT3& scale) { this->scl = scale; }
	//発射フラグのセット
	void SetTrigger(bool Trigger) { this->Trigger = Trigger; }
	//waveカウント
	void SetCount(int count) { this->count = count; }
#pragma endregion

	
	
private:
	std::unique_ptr<Object3d> bullet;
	ObjModel* bulModel = nullptr;


	XMVECTOR pos = { 0.0f,0.0f,-100.0f };
	XMVECTOR oldpos{ 0.0f,0.0f,0.0f };
	XMFLOAT3 scl = { 0.1f,0.1f,0.1f };
	XMFLOAT3 rot = { 0.0f,0.0f,0.0f };

	bool shot = false;
	int rebun = 0;
	float speed = 1.0f;
	float Loadtime = 0.0f;

	bool lost = false;

	XMVECTOR velocity_;
	XMMATRIX mat;
	XMMATRIX matvel;
	//debug
	bool d = false;
	float v3x = 0, v3y = 0, v3z = 0;
	bool fire = false;
	bool move = false;
	bool Trigger = false;

	//Reload
	bool ReloadFlag = false;
	float ShotT = 0;
	int ans = 0;
	//実像時間
	float time = 0.0f;
	bool drawF = false;
	//wav数
	int count = 0;
};