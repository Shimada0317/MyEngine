#include "GameOverScene.h"
#include"SceneManager.h"
#include"TitleScene.h"

GameOverScene::GameOverScene(SceneManager* sceneManager_)
	:BaseScene(sceneManager_)
{
}

void GameOverScene::Initialize(DirectXCommon* dxComon)
{
	Sprite::LoadTexture(101, L"Resources/GameOver.png");
	GameOver = Sprite::SpriteCreate(101, { 1.0f,1.0f });
}

void GameOverScene::SetPosSclRot()
{
	GameOver->SetSize({ size });
	GameOver->SetPosition({ pos });
}

void GameOverScene::Update()
{
	if (Mouse::GetInstance()->PushClick(0)|| Mouse::GetInstance()->PushClick(1)) {
		BaseScene* scene_ = new TitleScene(sceneManager_);
		sceneManager_->SetNextScene(scene_);
	}
	SetPosSclRot();
}

void GameOverScene::Draw(DirectXCommon* dxCommon)
{
	dxCommon->PreDraw();
	Sprite::PreDraw(dxCommon->GetCmdList());
	GameOver->Draw();
	Sprite::PostDraw();
	dxCommon->PostDraw();
}

void GameOverScene::Finalize()
{
	delete GameOver;
}
