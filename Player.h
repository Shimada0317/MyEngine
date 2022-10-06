#pragma once
#include"ObjModel.h"
#include"Object3d.h"
#include"Input.h"
#include"DebugText.h"
#include"Action.h"
#include"DebugCamera.h"
#include"Texture.h"
#include"TextureModel.h"
#include"WinApp.h"
#include<vector>
#include"Bullet.h"
#include"ParticleManager.h"

#include<memory>
#include<list>

const int BULL = 9;

class Player
{
public:

	void Initalize();

	void Set();

	void Effect();

	void Update(Bullet* bull[], int& Remaining);

	void ParticleDraw(ID3D12GraphicsCommandList* cmdeList);

	void Draw(ID3D12GraphicsCommandList* cmdList);

	void ObjDraw();

	void ImGuiDraw();

	void Finalize();

	void Attack();

	void MouthContoroll();

	const XMVECTOR& GetPosition() { return position; }

	void SetPosition(const XMVECTOR& position) { this->position = position; }

	const XMFLOAT3& GetRotation() { return rotation; }

	void SetRotation(const XMFLOAT3& rotation) { this->rotation = rotation; }

	const XMFLOAT3& GetScl() { return scale; }

	void SetScl(const XMFLOAT3& scale) { this->scale = scale; }


	const XMFLOAT2& GetRetPosition() { return retpos; }

	void SetRetPosition(const XMFLOAT2& position) { this->retpos = position; }

	const XMFLOAT2& GetRetSiz() { return retsize; }

	void SetRetSiz(const XMFLOAT2& scale) { this->retsize = scale; }

	XMVECTOR GetWorldPosition();
private:
	float OffsetX = 0.0f;
	float OffsetY = 0.0f;
	//プレイヤーオブジェクトとモデル
	std::unique_ptr<Object3d> player;
	ObjModel* playerModel = nullptr;
	//発砲時のエフェクト
	ParticleManager* part;

	Input* input = nullptr;
	DebugText* debugtext = nullptr;
	DebugCamera* camera = nullptr;

	XMVECTOR position = { 0.0f,0.0f,0.1f };
	XMVECTOR backPlayerPos{ 0.0f,0.0f,0.0f };
	XMVECTOR oldPos = { 0.0f,0.0f,0.0f };
	XMFLOAT3 rotation = { 0.0f,0.0f,0.0f };
	XMFLOAT3 scale = { 0.1f,0.1f,0.1f };
	XMMATRIX mat;
	//カメラ
	XMFLOAT3 cameraTarget = { 0.0f,1.0f,0.0f };
	XMFLOAT3 camerapos = { 0.0f,0.0f,0.0f };
	XMFLOAT3 Eye_pos = {0.0f,0.0f,0.0f};
	XMFLOAT3 Target_pos = {0.0f,0.0f,0.0f};
	XMFLOAT3 up = { 0.0f,1.0f,0.0f };

	XMFLOAT4 color = { 1,1,1,1 };

	XMFLOAT2 retpos = { 0.0f,0.0f };
	XMFLOAT2 retsize = { 64.0f,64.0f };

	POINT pos;
	WinApp* winapp = nullptr;


	float time = 0.0f;
	bool particle = false;
	bool shot = false;
	int fire = 0;

	int Hp = 5;
};

