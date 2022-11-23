#pragma once
#include "BaseScene.h"
#include <DirectXMath.h>
#include"Robot.h"
#include"Body.h"
#include"Head.h"
#include"DebugCamera.h"
#include"Object3d.h"
#include"ObjModel.h"
#include"ObjParticle.h"
#include"ParticleManager.h"
#include"Player.h"
#include"Light.h"
#include"middle.h"
#include"Mouse.h"
#include<memory>

class DebugScene :
    public BaseScene
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
private:
	static const int debugTextNumber = 0;

public://メンバ変数
	DebugScene(SceneManager* sceneManager_);

	void Initialize(DirectXCommon* dxComon) override;

	void SetPosSclRot();

	void Updata() override;

	void Draw(DirectXCommon* dxCommon)override;

	void ChangeViewPort(XMMATRIX& mat);

	void MouthContoroll();

	void Finalize() override;
private:
	std::unique_ptr<Robot>rob[3];
	std::unique_ptr<Body>bob;
	std::unique_ptr<Head>head;
	std::unique_ptr<Object3d> bo = nullptr;
	std::unique_ptr<Player>player;
	std::list<std::unique_ptr<ObjParticle>>part;
	std::unique_ptr<Sprite> spriteRet;

	Camera* camera = nullptr;

	RailCamera* cam;
	XMVECTOR vel = { 0.0f,0.0f,0.0f };
	XMFLOAT3 Eye_rot = { 0.0f,0.0f,0.0f };
	float kBulletSpeed = 0;
	Bullet* bull[9];
	Light* light = nullptr;

	ObjModel* bomodel = nullptr;
	int Remaining = 2;

	XMVECTOR position = { 0.0f,0.0f,0.1f };
	XMFLOAT3 rotation = { 0.0f,0.0f,0.0f };
	XMFLOAT3 scale = { 0.1f,0.1f,0.1f };

	XMFLOAT4 color = { 1,1,1,1 };
	XMFLOAT4 spCol = { 1,1,1,1 };
	XMFLOAT2 anc = { 0.5f,0.5f };
	XMFLOAT2 retpos = { 640.0f,360.0f };
	XMFLOAT2 retsize = { 64.0f,64.0f };

	XMVECTOR allpos[3];
	XMVECTOR offset = { 0,0,1.0f };
	XMVECTOR velocity = { 0.0f,0.0f,0.0f };

	bool jump = false;
	float addgrav = 0.4f;
	float time = 0.0f;
	float gravity = 0.0f;

	std::unique_ptr<middle>mid;

	//ワールド
	XMVECTOR playerWorldPos = { 0.0f,0.0f,0.0f };
	XMMATRIX mat;
};


