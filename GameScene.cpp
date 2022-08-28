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
	

	
	////スプライトの読み込み
	Sprite::LoadTexture(1, L"Resources/background.png");
	
	title = Sprite::SpriteCreate(1, { 1.0f,1.0f });

	//モデルの読み込み

	playermodel = ObjModel::CreateFromOBJ("skydome");
	sphere = Object3d::Create();
	sphere->SetModel(playermodel);


	ground = ObjModel::CreateFromOBJ("city");
	groundObj = Object3d::Create();

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

	mid =std::make_unique <middle>();
	mid->Initialize();
	patern = mid->GetPatern();

	light = Light::Create();

}

void GameScene::SetPosSclRot()
{

	sphere->SetRotation({ 0,0,0 });
	sphere->SetPosition({ pos });
	sphere->SetScale({ 4.0f,4.0f,4.0f });



	groundObj->SetPosition({ ground_pos });
	groundObj->SetScale({ ground_scl });

	title->SetSize({ screen_size });
	patern = mid->GetPatern();
};

void GameScene::AllUpdate()
{
	sphere->Update();
	groundObj->Update();
	particle->Update();
	mid->Update();
}

void GameScene::Update()
{
	if (patern == 5) {
		BaseScene* scene_ = new ClearScene(sceneManager_);
		sceneManager_->SetNextScene(scene_);
	}

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
	//delete sphere;
	delete dxCommon;
	delete light;
	delete Object;
	delete model;
}