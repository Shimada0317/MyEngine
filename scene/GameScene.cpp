
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

	//lightGroup = LightGroup::Create();
	//Object3d::SetLightGroup(lightGroup);
	light = Light::Create();
	Object3d::SetLight(light);

	//モデルの読み込み

	playermodel = ObjModel::CreateFromOBJ("skydome");
	sphere = Object3d::Create();
	sphere->SetModel(playermodel);


	ground = ObjModel::CreateFromOBJ("Field2",true);
	groundObj = Object3d::Create(ground);

	worldmodel = ObjModel::CreateFromOBJ("world", true);
	world = Object3d::Create(worldmodel);

	mid =std::make_unique <middle>();
	mid->Initialize();
	patern = mid->GetPatern();

	playerHp = mid->GetHp();
	oldHp = playerHp;

	postEffect = new PostEffect();
	postEffect->Initialize();
	
	clearT = 0;
}

void GameScene::SetPosSclRot()
{
	sphere->SetRotation({ 0,0,0 });
	sphere->SetPosition( pos );
	sphere->SetScale({ 4.0f,4.0f,4.0f });

	groundObj->SetPosition( ground_pos );
	groundObj->SetScale({ ground_scl });

	world->SetPosition(worldPos);
	world->SetScale(worldScl);

	patern = mid->GetPatern();
	playerHp = mid->GetHp();
	
};

void GameScene::AllUpdata()
{
	//lightGroup->Update();
	sphere->Updata();
	groundObj->Updata();
	world->Updata();
	mid->Updata();
}

void GameScene::Updata()
{
	if (oldHp > playerHp) {
		post = true;
		oldHp = playerHp;
	}
	if (post == true) {
		col.x=0.7f;
		if (col.x >= 0.5f) {
			post = false;
		}
	}
	if (post == false) {
		col.x -= 0.1f;
		if (col.x <= 0) {
			col.x = 0;
		}
	}


	if (playerHp<=0) {
		BaseScene* scene_ = new GameOverScene(sceneManager_);
		sceneManager_->SetNextScene(scene_);
	}

	if (patern >= 10) {
		clearT += 0.1f;
		
	}

	if (clearT >= 10) {
		BaseScene* scene_ = new ClearScene(sceneManager_);
		sceneManager_->SetNextScene(scene_);
	}

	SetPosSclRot();
	postEffect->Update(col);
	AllUpdata();
}

void GameScene::ObjDraw(DirectXCommon* dxCommon)
{

	////オブジェクト前処理
	Object3d::PreDraw(dxCommon->GetCmdList());
	sphere->Draw();
	groundObj->Draw();
	world->Draw();
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
	delete worldmodel;
	mid.reset();
	delete postEffect;
}

