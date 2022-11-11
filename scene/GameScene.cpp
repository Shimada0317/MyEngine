
#include "GameScene.h"
#include<cassert>
#include <sstream>
#include <iomanip>
#include"Action.h"
#include"SceneManager.h"
#include"FbxLoader.h"
#include"FbxObject3d.h"
#include"ClearScene.h"
#include"GameOverScene.h"
#include"DebugScene.h"
#include"ModelManager.h"


using namespace DirectX;

GameScene::GameScene(SceneManager* sceneManager_)
	:BaseScene(sceneManager_)
{

}

void GameScene::Initialize(DirectXCommon* dxComon)
{
	
	ModelManager::GetInstance()->Initialize();
	
	////スプライトの読み込み

	
	//モデルの読み込み

	playermodel = ObjModel::CreateFromOBJ("skydome");
	sphere = Object3d::Create();
	sphere->SetModel(playermodel);


	ground = ObjModel::CreateFromOBJ("Field");
	groundObj = Object3d::Create();

	groundObj->SetModel(ground);
	

	mid =std::make_unique <middle>();
	mid->Initialize();
	patern = mid->GetPatern();

	light = Light::Create();
	playerHp = mid->GetHp();

	postEffect = new PostEffect();
	postEffect->Initialize();
}

void GameScene::SetPosSclRot()
{
	sphere->SetRotation({ 0,0,0 });
	sphere->SetPosition( pos );
	sphere->SetScale({ 4.0f,4.0f,4.0f });

	groundObj->SetPosition( ground_pos );
	groundObj->SetScale({ ground_scl });

	patern = mid->GetPatern();
	playerHp = mid->GetHp();
};

void GameScene::AllUpdata()
{
	sphere->Updata();
	groundObj->Updata();
	mid->Updata();
}

void GameScene::Updata()
{
	/*if (Input::GetInstance()->PushKey(DIK_A)) {
		BaseScene* scene_ = new GameOverScene(sceneManager_);
		sceneManager_->SetNextScene(scene_);
	}*/

	/*if (patern >= 5) {
		BaseScene* scene_ = new ClearScene(sceneManager_);
		sceneManager_->SetNextScene(scene_);
	}*/

	SetPosSclRot();
	AllUpdata();
}

void GameScene::ObjDraw(DirectXCommon* dxCommon)
{

	////オブジェクト前処理
	Object3d::PreDraw(dxCommon->GetCmdList());
	sphere->Draw();
	groundObj->Draw();
	////human3d->Draw();
	////オブジェクト後処理
	Object3d::PostDraw();
	mid->Draw(dxCommon);

}

void GameScene::SpriteDraw(DirectXCommon* dxCommon)
{
	Sprite::PreDraw(dxCommon->GetCmdList());
	mid->SpriteDraw();
	Sprite::PostDraw();
}

void GameScene::ImgDraw()
{
	mid->ImGuiDraw();
}

void GameScene::PostEffectDraw(DirectXCommon* dxCommon)
{
	
	postEffect->PreDrawScene(dxCommon->GetCmdList());
	ObjDraw(dxCommon);
	SpriteDraw(dxCommon);
	postEffect->PostDrawScene(dxCommon->GetCmdList());

	dxCommon->PreDraw();
	postEffect->Draw(dxCommon->GetCmdList());
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
	delete ground;
	delete playermodel;
	mid.reset();
	delete postEffect;
}

