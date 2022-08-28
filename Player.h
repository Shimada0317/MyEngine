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

#include<memory>

class Player
{
public:

	void Initalize();

	void Set();

	void Update();

	void Draw(ID3D12GraphicsCommandList* cmdList);

	void ObjDraw();

	void ImGuiDraw();

	void Finalize();

	void MouthContoroll();

	const XMFLOAT3& GetPosition() { return position; }

	void SetPosition(XMFLOAT3 position) { this->position = position; }

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


	Input* input = nullptr;
	DebugText* debugtext = nullptr;
	DebugCamera* camera = nullptr;

	XMFLOAT3 position = { 0.0f,0.2f,0.0f };
	XMFLOAT3 rotation = { 0.0f,0.0f,0.0f };
	XMFLOAT3 scale = { 0.2f,0.2f,0.2f };

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

};

