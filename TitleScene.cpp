//#include "TitleScene.h"
//#include"SceneManager.h"
//#include<cassert>
//#include <sstream>
//#include <iomanip>
//#include"GameScene.h"
//
//using namespace DirectX;
//
//TitleScene::TitleScene(SceneManager* sceneManager_)
//	:BaseScene(sceneManager_)
//{
//
//}
//
//void TitleScene::Initialize(DirectXCommon* dxComon)
//{
//
//	//post = new PostEffect();
//	//post->Initialize();
//
//	////スプライトの読み込み
//	Sprite::LoadTexture(1, L"Resources/tst1.png");
//	title = Sprite::SpriteCreate(1, { 1.0f,1.0f });
//
//	enemy = std::make_unique<Robot>();
//	enemy->Initialize();
//
//	//モデルの読み込み
//	/*playermodel = Model::LoadFromObJ("player");
//	player3d = Object3d::Create();
//	player3d->SetModel(playermodel);*/
//
//}
//
//void TitleScene::SetPosSclRot()
//{
//	//プレイヤー
//	/*player3d->SetRotation({ 0,450,-270 });
//	player3d->SetPosition({ 0,0,0 });
//	player3d->SetScale({0.5f,0.5f,0.5f});*/
//
//
//	title->SetSize({retsize});
//	title->SetPosition({ retpos });
//
//
//
//	/*GetCursorPos(&pos);
//
//	ScreenToClient(FindWindowA("DirectXGame", nullptr), &pos);
//
//	retpos.x = pos.x;
//	retpos.y = pos.y;*/
//}
//
//void TitleScene::Update()
//{
//	//DirectX毎フレーム処理 ここから
//
//	if (Input::GetInstance()->TriggerKey(DIK_RETURN)) {
//		//シーン切り替え
//		BaseScene* scene_ = new GameScene(sceneManager_);
//		sceneManager_->SetNextScene(scene_);
//	}
//	SetPosSclRot();
//	enemy->Update();
//}
//
//void TitleScene::Draw(DirectXCommon* dxCommon)
//{
//	dxCommon->PreDraw();
//	//post->PreDrawScene(dxCommon->GetCmdList());
//
//
//	Sprite::PreDraw(dxCommon->GetCmdList());
//	//title->Draw();
//	Sprite::PostDraw();
//	Object3d::PreDraw(dxCommon->GetCmdList());
//	enemy->Draw(dxCommon);
//	Object3d::PostDraw();
//
//
//	// 
//	//post->PostDrawScene(dxCommon->GetCmdList());
//	
//	//post->Draw(dxCommon->GetCmdList());
//	dxCommon->PostDraw();
//}
//
//void TitleScene::Finalize()
//{
//	delete title;
//	delete dxCommon;
//	enemy.reset();
//	//delete particle;
//}