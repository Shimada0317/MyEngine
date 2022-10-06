//#include "ClearScene.h"
//#include "TitleScene.h"
//#include"SceneManager.h"
//
//ClearScene::ClearScene(SceneManager* sceneManager_)
//	:BaseScene(sceneManager_)
//{
//}
//
//void ClearScene::Initialize(DirectXCommon* dxComon)
//{
//	Sprite::LoadTexture(100, L"Resources/Clear.png");
//	Clear = Sprite::SpriteCreate(100, { 1.0f,1.0f });
//}
//
//void ClearScene::SetPosSclRot()
//{
//	Clear->SetSize(retsize);
//	Clear->SetPosition(retpos);
//}
//
//void ClearScene::Update()
//{
//	if (Input::GetInstance()->PushKey(DIK_SPACE)) {
//		BaseScene* scene_ = new TitleScene(sceneManager_);
//		sceneManager_->SetNextScene(scene_);
//	}
//}
//
//void ClearScene::Draw(DirectXCommon* dxCommon)
//{
//	dxCommon->PreDraw();
//	Sprite::PreDraw(dxCommon->GetCmdList());
//	Clear->Draw();
//	Sprite::PostDraw();
//	dxCommon->PostDraw();
//}
//
//void ClearScene::Finalize()
//{
//}
