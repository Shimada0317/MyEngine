#include "GameScene.h"
#include<cassert>
#include <sstream>
#include <iomanip>
#include"Action.h"
#include"SceneManager.h"
#include"FbxLoader.h"
#include"FbxObject3d.h"


using namespace DirectX;

GameScene::GameScene(SceneManager* sceneManager_)
	:BaseScene(sceneManager_)
{

}

void GameScene::Initialize(DirectXCommon* dxComon)
{
	
	//Sprite::LoadTexture(100, L"Resources/background.png");
	//postEffect = new PostEffect();
	//postEffect->Initialize();

	/*camera = new DebugCamera(WinApp::window_width, WinApp::window_height);
	Object3d::SetCamera(camera);*/
	
	////スプライトの読み込み
	Sprite::LoadTexture(1, L"Resources/background.png");
	
	title = Sprite::SpriteCreate(1, { 1.0f,1.0f });

	
	//postEffect->InitializeSprite();

	//モデルの読み込み

	playermodel = ObjModel::CreateFromOBJ("skydome");
	sphere->CreateGraphicsPipeline(L"Resources/shaders/BasicVS.hlsl", L"Resources/shaders/BasicPS.hlsl");
	sphere = Object3d::Create();
	sphere->SetModel(playermodel);


	ground = ObjModel::CreateFromOBJ("city");
	groundObj = Object3d::Create();
	groundObj->CreateGraphicsPipeline(L"Resources/shaders/BasicVS.hlsl", L"Resources/shaders/BasicPS.hlsl");
	groundObj->SetModel(ground);
	

	//モデル名を指定してファイル読み込み
	//model = FbxLoader::GetInstance()->LoadModelFromFile("boneTest");

	//FbxObject3d::SetDevice(dxComon->GetDev());
	//FbxObject3d::SetCamera(camera);
	//FbxObject3d::CreateGraphicPipeline();

	//Object = new FbxObject3d;
	//Object->Initialize();
	//Object->SetModel(model);

	particle = ParticleManager::Create();
	particle->Update();

	mid = new middle;
	mid->Initialize();

	light = Light::Create();

	//light->SetLightColor({ 1,1,1 });

	//Object3d::SetLight(light);
}

void GameScene::SetPosSclRot()
{

	sphere->SetRotation({ 0,0,0 });
	sphere->SetPosition({ pos });
	sphere->SetScale({ 4.0f,4.0f,4.0f });

	/*camera->SetDistance({ cameradistance });
	camera->SetEye({ cameraEye });
	camera->SetTarget({ cameraTarget });*/

	groundObj->SetPosition({ ground_pos });
	groundObj->SetScale({ ground_scl });

	/*Object->SetPosition({ obj_pos });
	Object->SetRotation({ obj_rot });*/

	title->SetSize({ screen_size });

	//for (int i = 0; i < 1000; i++) {
	//	const float rnd_pos = 10.0f;
	//	XMFLOAT3 pos{};
	//	pos.x = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
	//	pos.y = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
	//	pos.z = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;

	//	const float rnd_vel = 0.1f;
	//	XMFLOAT3 vel{};
	//	vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
	//	vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
	//	vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

	//	XMFLOAT3 acc{};
	//	const float rnd_acc = 0.001f;
	//	acc.y = -(float)rand() / RAND_MAX * rnd_acc;

	//	particle->Add(60, pos, vel, acc, 1.0f, 0.0f);
	//	break;
	//}

	//static XMVECTOR lightDir = { 0,1,5,0 };

	//if (Input::GetInstance()->PushKey(DIK_W)) { lightDir.m128_f32[1] += 1.0f; }
	//else if (Input::GetInstance()->PushKey(DIK_S)) { lightDir.m128_f32[1] -= 1.0f; }
	//if (Input::GetInstance()->PushKey(DIK_D)) { lightDir.m128_f32[0] += 1.0f; }
	//else if (Input::GetInstance()->PushKey(DIK_A)) { lightDir.m128_f32[0] -= 1.0f; }
};

void GameScene::AllUpdate()
{
//	camera->Update();
	sphere->Update();
//	Object->Update();
	groundObj->Update();
	particle->Update();
	mid->Update();
	//light->Update();
}

void GameScene::Update()
{
	SetPosSclRot();
	AllUpdate();
}

void GameScene::ObjDraw(DirectXCommon* dxCommon)
{

	ParticleManager::PreDraw(dxCommon->GetCmdList());
	//particle->Draw();
	ParticleManager::PostDraw();
	////オブジェクト前処理
	Object3d::PreDraw(dxCommon->GetCmdList());
	sphere->Draw();
	groundObj->Draw();
	////human3d->Draw();
	////オブジェクト後処理
	mid->Draw(dxCommon->GetCmdList());
	Object3d::PostDraw();


	//Object->Draw(dxCommon->GetCmdList());
}

void GameScene::SpriteDraw(DirectXCommon* dxCommon)
{
	Sprite::PreDraw(dxCommon->GetCmdList());
	//title->Draw();
	mid->SpriteDraw();
	Sprite::PostDraw();
}

void GameScene::ImgDraw()
{
	mid->ImGuiDraw();
}

void GameScene::PostEffectDraw(DirectXCommon* dxCommon)
{
	dxCommon->PreDraw();
	//postEffect->PreDrawScene(dxCommon->GetCmdList());
	ObjDraw(dxCommon);
	SpriteDraw(dxCommon);
	//postEffect->PostDrawScene(dxCommon->GetCmdList());


	//postEffect->Draw(dxCommon->GetCmdList());
	ImgDraw();
	//描画後処理
	dxCommon->PostDraw();
}

void GameScene::Draw(DirectXCommon* dxCommon)
{
	PostEffectDraw(dxCommon);
}

void GameScene::Finalize()
{
	delete title;
	delete sphere;
	delete groundObj;
	//delete sphere;
	delete winApp;
	delete dxCommon;
	delete light;
	delete Object;
	delete model;
}