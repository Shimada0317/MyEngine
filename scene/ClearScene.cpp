#include "ClearScene.h"
#include "TitleScene.h"
#include"SceneManager.h"

ClearScene::ClearScene(SceneManager* sceneManager_)
	:BaseScene(sceneManager_)
{
}

void ClearScene::Initialize(DirectXCommon* dxComon)
{
	Sprite::LoadTexture(100, L"Resources/title.png");
	Clear = Sprite::SpriteCreate(100, { 1.0f,1.0f });
}

void ClearScene::SetPosSclRot()
{
	Clear->SetSize(retsize);
	Clear->SetPosition(retpos);
}

void ClearScene::Updata()
{
	if (Input::GetInstance()->PushKey(DIK_RETURN)) {
		BaseScene* scene_ = new TitleScene(sceneManager_);
		sceneManager_->SetNextScene(scene_);
	}
	SetPosSclRot();
}

void ClearScene::Draw(DirectXCommon* dxCommon)
{
	dxCommon->PreDraw();
	Sprite::PreDraw(dxCommon->GetCmdList());
	Clear->Draw();
	Sprite::PostDraw();
	dxCommon->PostDraw();
}

void ClearScene::Finalize()
{
	delete Clear;
}
