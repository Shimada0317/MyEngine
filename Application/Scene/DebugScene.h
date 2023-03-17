#pragma once
#include "BaseScene.h"
#include <DirectXMath.h>
#include"Enemy.h"
#include"DebugCamera.h"
#include"Object3d.h"
#include"ObjModel.h"
#include"ObjParticle.h"
#include"ParticleManager.h"
#include"Player.h"
#include"Light.h"
#include"Mouse.h"
#include"Texture.h"
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

	void Update() override;

	void Draw(DirectXCommon* dxCommon)override;

	void ImguiDraw();

	void Finalize() override;
private:
	std::unique_ptr<Object3d> Sphere;
	std::unique_ptr<Object3d> Ground;
	std::unique_ptr<Object3d> sphere;
	std::unique_ptr<Object3d> wall;

	ObjModel* SphereModel;
	ObjModel* GroundModel;


	Camera* camera = nullptr;
	Light* light = nullptr;

	XMFLOAT3 cameraEye = { 0.0f,0.0f,-30.0f };
	XMFLOAT3 cameraTarget = { 0.0f,0.0f,10.0f };

	//球体のステータス
	XMVECTOR SpherePos = { 0,0,0 };
	XMVECTOR SphereWPos = { 0,0,0 };
	XMFLOAT3 SphereScl = { 4.0f,4.0f,4.0f };
	XMFLOAT3 SphereRot = { 0,0,0 };
	//フィールドのステータス
	XMVECTOR GroundPos = { 0.0f,-6.0f,0.0f };
	XMFLOAT3 GroundScl = { 14,4,14 };
	XMFLOAT3 GroundRot = { 0,0,0 };

	XMVECTOR spherePos = { 0,0,0 };
	XMFLOAT3 sphereRot = { 0,0,0 };
	XMFLOAT3 sphereScl = { 4.0f,4.0f,4.0f };

	XMFLOAT3 PointLightsPos = { 0,0,0 };
	float PoinLightColor[3] = { 1,1,1 };
	float PointLightAtten[3] = { 0.03f,0.01f,0.01f };

	LightGroup* lightGroupe;
	XMVECTOR lightDir = { 0,0,0,0 };

	XMFLOAT3 SpotLightDir = { 0,-1,0};
	XMFLOAT3 SpotLightPos = { 0,5,0 };
	XMFLOAT3 SpotLightColor = { 1,1,1 };
	XMFLOAT3 SpotLightAtten = { 0.f,0.f,0.f };
	XMFLOAT2 SpotLightFactorAngle = { 10.0f,20.0f };

	XMFLOAT3 SpotLightDir2 = { 0,-1,0 };
	XMFLOAT3 SpotLightPos2 = { 0,5,0 };
	XMFLOAT3 SpotLightColor2 = { 1,1,1 };
	XMFLOAT3 SpotLightAtten2 = { 0.f,0.f,0.f };
	XMFLOAT2 SpotLightFactorAngle2 = { 20.0f,30.0f };
};


