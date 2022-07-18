#pragma once
#include"ObjModel.h"
#include"Object3d.h"
#include"Input.h"
#include"DebugText.h"
#include"Action.h"
#include"DebugCamera.h"
#include"Texture.h"
#include"TextureModel.h"
#include<vector>

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

	const XMFLOAT3& GetPosition() { return position; }
private:
	ObjModel* model = nullptr;
	ObjModel* modelnext = nullptr;

	Object3d* player = nullptr;
	Object3d* playernext = nullptr;
	
	Texture* tex = nullptr;
	TextureModel* texmodel = nullptr;

	Input* input = nullptr;
	DebugText* debugtext = nullptr;
	DebugCamera* camer = nullptr;

	XMFLOAT3 position = { 0.0f,0.0f,-17.0f };
	XMFLOAT3 rotation = { 0.0f,0.0f,-10.0f };
	XMFLOAT3 scael = { 0.5f,0.5f,0.5f };
	XMFLOAT3 cameraEye = { 0.0f,0.0f,-10.0f };
	XMFLOAT3 cameraTarget = { 0.0f,1.0f,0.0f };

	XMFLOAT3 camerapos = { 0.0f,0.0f,0.0f };

	XMFLOAT3 Eye_pos = {};
	XMFLOAT3 Target_pos = {};

	XMFLOAT3 size = { 0,0,0};
	XMFLOAT4 color = { 1,1,1,1 };

	bool Changeshader = false;
	bool stopF = false;
};

