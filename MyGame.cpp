#include "MyGame.h"
#include"GameScene.h"
#include"TitleScene.h"

void MyGame::Initialize()
{
	//基底クラスの初期化
	Framework::Initialize();
	BaseScene* scene_ = new TitleScene(sceneManager_);
	//シーンマネージャーに最初のシーンをセット
	sceneManager_->SetNextScene(scene_);

}

void MyGame::Update()
{
	//基底クラスの更新
	Framework::Update();

}

void MyGame::Draw()
{
	Framework::Draw();
}

void MyGame::Finalize()
{
	//基底クラスの終了
	Framework::Finalize();

}
