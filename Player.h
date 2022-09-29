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

class Player
{
public:

	void Initalize();

	void Set();

	void Update();

	void ParticleDraw(ID3D12GraphicsCommandList* cmdeList);

	void Draw(ID3D12GraphicsCommandList* cmdList);

	void ObjDraw();

	void ImGuiDraw();

	void Finalize();

	void Attack();

	void MouthContoroll();

	const XMVECTOR& GetPosition() { return position; }

	void SetPosition(XMVECTOR position) { this->position = position; }

	const XMFLOAT3& GetRotation() { return rotation; }

	void SetRotation(XMFLOAT3 rotation) { this->rotation = rotation; }

	const XMFLOAT3& GetScl() { return scale; }

	void SetScl(XMFLOAT3 scale) { this->scale = scale; }


	const XMFLOAT2& GetRetPosition() { return retpos; }

	void SetRetPosition(XMFLOAT2 position) { this->retpos = position; }

	const XMFLOAT2& GetRetSiz() { return retsize; }

	void SetRetSiz(XMFLOAT2 scale) { this->retsize = scale; }

	XMVECTOR GetWorldPosition();
private:
	float OffsetX = 0.0f;
	float OffsetY = 0.0f;

	Sprite* reticle = nullptr;
	std::unique_ptr<Object3d> player;
	ObjModel* playerModel = nullptr;
	ParticleManager* part;

	std::list<std::unique_ptr<Bullet>> bullets_;
	std::unique_ptr<Bullet> newBullet;
	Input* input = nullptr;
	DebugText* debugtext = nullptr;
	DebugCamera* camera = nullptr;

	XMVECTOR position = { 0.0f,0.0f,0.1f };
	XMFLOAT3 rotation = { 0.0f,0.0f,0.0f };
	XMFLOAT3 scale = { 0.1f,0.1f,0.1f };
	XMMATRIX mat;

	XMFLOAT3 cameraTarget = { 0.0f,1.0f,0.0f };
	XMFLOAT3 camerapos = { 0.0f,0.0f,0.0f };
	XMFLOAT3 Eye_pos = {0.0f,0.0f,0.0f};
	XMFLOAT3 Target_pos = {0.0f,0.0f,0.0f};
	XMFLOAT3 up = { 0.0f,1.0f,0.0f };

	XMFLOAT3 size = { 0,0,0};
	XMFLOAT4 color = { 1,1,1,1 };

	XMFLOAT2 retpos = { 0.0f,0.0f };
	XMFLOAT2 retsize = { 64.0f,64.0f };

	POINT pos;
	WinApp* winapp = nullptr;


	float time = 0.0f;
	bool particle = false;
};

