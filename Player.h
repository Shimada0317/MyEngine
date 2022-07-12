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

	void Finalize();

private:
	ObjModel* model = nullptr;
	Object3d* player = nullptr;
	
	Texture* tex = nullptr;
	TextureModel* texmodel = nullptr;

	Input* input = nullptr;
	DebugText* debugtext = nullptr;
	DebugCamera* camer = nullptr;

	XMFLOAT3 position = { 0.0f,0.0f,0.0f };
	XMFLOAT3 cameraEye = { 0.0f,0.0f,-10.0f };
	XMFLOAT3 cameraTarget = { 0.0f,1.0f,0.0f };

	XMFLOAT3 camerapos = { 0.0f,0.0f,0.0f };

	XMFLOAT3 Eye_pos = {};
	XMFLOAT3 Target_pos = {};

	bool stopF = false;
};

