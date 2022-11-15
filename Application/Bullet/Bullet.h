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
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;
public:
	~Bullet();

	void Initialize();

	void Stanby(const XMVECTOR& playerPos,const XMVECTOR& vel);

	void bun(XMVECTOR& pos, XMVECTOR position, float& speed, bool& shot, int& Remaining,bool& retime);

	void Shot(XMVECTOR& pos, XMFLOAT2 position, int speed, bool& shot);

	void debug(XMFLOAT3& pos, XMFLOAT3 posiiton, int speed,bool& d);

	void Test(const XMVECTOR& GWorldPos, const XMVECTOR& PWorldPos, const XMFLOAT3& Eye_Rot);

	void Set();

	void ShotBefore(const XMVECTOR& startPos, const XMVECTOR& velocity);

	bool CheckOk();

	void TriggerOn();

	void ShotAfter(const XMVECTOR& baclplayerpos, const XMVECTOR& playerpos,const XMVECTOR&PlayerWorldPos,int& remaining);

	void Hit();
	
	void Updata();

	void Draw();

	void ImgUiDraw();

	void Finalize();

#pragma region Get
	// ���W�̃Q�b�g
	const XMVECTOR& GetPosition() { return pos; }
	//�p�x�̃Q�b�g
	const XMFLOAT3& GetRotation() { return rot; }
	//�X�P�[���̃Q�b�g
	const XMFLOAT3& GetScl() { return scl; }
	//���˃t���O�̃Q�b�g
	bool GetTrigger() { return Trigger; }
#pragma endregion

#pragma region Set
	//���W�̃Z�b�g
	void SetPosition(XMVECTOR position) { this->pos = position; }
	//�p�x�̃Z�b�g
	void SetRotation(XMFLOAT3 rotation) { this->rot = rotation; }
	//�X�P�[���̃Z�b�g
	void SetScl(const XMFLOAT3& scale) { this->scl = scale; }
	//���˃t���O�̃Z�b�g
	void SetTrigger(bool Trigger) { this->Trigger = Trigger; }
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
};