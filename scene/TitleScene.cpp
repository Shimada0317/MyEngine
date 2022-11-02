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

	Sprite::LoadTexture(100, L"Resources/white1x1.png");
	npost = new NewPostEffect();
	npost->Initilaize();


	
}

void TitleScene::SetPosSclRot()
{
	title->SetSize({ titlesize });
	title->SetPosition({ titlepos });
}

void TitleScene::Update()
{
	Action::GetInstance()->PlayerMove2d(titlesize,1.0f);

	//DirectX毎フレーム処理 ここから
	timer += 1;
	if (timer >= 10) {
		if (Input::GetInstance()->TriggerKey(DIK_RETURN)) {
			//シーン切り替え
			BaseScene* scene_ = new GameScene(sceneManager_);
			sceneManager_->SetNextScene(scene_);
			timer = 0;
		}
	}
	SetPosSclRot();
}

void TitleScene::Draw(DirectXCommon* dxCommon)
{
	npost->PreDrawScene(dxCommon->GetCmdList());
//	post->PreDrawScene(dxCommon->GetCmdList());
	Sprite::PreDraw(dxCommon->GetCmdList());
	title->Draw();
	Sprite::PostDraw();
	npost->PostDrawScene(dxCommon->GetCmdList());
	//post->PostDrawScene(dxCommon->GetCmdList());
	dxCommon->PreDraw();
	npost->Draw(dxCommon->GetCmdList());
//	post->Draw(dxCommon->GetCmdList());
	dxCommon->PostDraw();
}

void TitleScene::Finalize()
{
	delete npost;
	delete title;
	delete dxCommon;
}