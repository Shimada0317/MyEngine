#include "TitleScene.h"
#include"SceneManager.h"
#include<cassert>
#include <sstream>
#include <iomanip>
#include"GameScene.h"

using namespace DirectX;

TitleScene::TitleScene(SceneManager* sceneManager_)
	:BaseScene(sceneManager_)
{

}

void TitleScene::Initialize(DirectXCommon* dxComon)
{
	////スプライトの読み込み
	Sprite::LoadTexture(1, L"Resources/tst.png");
	title = Sprite::SpriteCreate(1, { 1.0f,1.0f });
}

void TitleScene::SetPosSclRot()
{
	title->SetSize({ titlesize });
	title->SetPosition({ titlepos });
}

void TitleScene::Update()
{
	//DirectX毎フレーム処理 ここから

	if (Input::GetInstance()->TriggerKey(DIK_RETURN)) {
		//シーン切り替え
		BaseScene* scene_ = new GameScene(sceneManager_);
		sceneManager_->SetNextScene(scene_);
	}
	SetPosSclRot();
}

void TitleScene::Draw(DirectXCommon* dxCommon)
{
	dxCommon->PreDraw();
	//post->PreDrawScene(dxCommon->GetCmdList());
	Sprite::PreDraw(dxCommon->GetCmdList());
	title->Draw();
	Sprite::PostDraw();


	// 
	//post->PostDrawScene(dxCommon->GetCmdList());
	
	//post->Draw(dxCommon->GetCmdList());
	dxCommon->PostDraw();
}

void TitleScene::Finalize()
{
	delete title;
	delete dxCommon;
}