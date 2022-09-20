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

	post = new PostEffect();
	post->Initialize();

	////スプライトの読み込み
	Sprite::LoadTexture(1, L"Resources/tst1.png");
	title = Sprite::SpriteCreate(1, { 1.0f,1.0f });

	//モデルの読み込み
	/*playermodel = Model::LoadFromObJ("player");
	player3d = Object3d::Create();
	player3d->SetModel(playermodel);*/


	particle = ParticleManager::Create();
	particle->Update();

}

void TitleScene::SetPosSclRot()
{
	//プレイヤー
	/*player3d->SetRotation({ 0,450,-270 });
	player3d->SetPosition({ 0,0,0 });
	player3d->SetScale({0.5f,0.5f,0.5f});*/


	title->SetSize({retsize});
	title->SetPosition({ retpos });

	for (int i = 0; i < 1000; i++) {
		const float rnd_pos = 10.0f;
		XMFLOAT3 pos{};
		pos.x = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		pos.y = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		pos.z = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;

		const float rnd_vel = 0.1f;
		XMFLOAT3 vel{};
		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

		XMFLOAT3 acc{};
		const float rnd_acc = 0.001f;
		acc.y = -(float)rand() / RAND_MAX * rnd_acc;

		particle->Add(60, pos, vel, acc, 1.0f, 0.0f);
		break;
	}

	/*GetCursorPos(&pos);

	ScreenToClient(FindWindowA("DirectXGame", nullptr), &pos);

	retpos.x = pos.x;
	retpos.y = pos.y;*/
}

void TitleScene::Update()
{
	//DirectX毎フレーム処理 ここから

	Action::GetInstance()->PlayerMove2d(retsize, 100);

	if (Input::GetInstance()->TriggerKey(DIK_RETURN)) {
		//シーン切り替え
		BaseScene* scene_ = new GameScene(sceneManager_);
		sceneManager_->SetNextScene(scene_);
	}
	SetPosSclRot();
	particle->Update();
}

void TitleScene::Draw(DirectXCommon* dxCommon)
{
	post->PreDrawScene(dxCommon->GetCmdList());
	Sprite::PreDraw(dxCommon->GetCmdList());

	title->Draw();
	Sprite::PostDraw();
	post->PostDrawScene(dxCommon->GetCmdList());

	dxCommon->PreDraw();
	
	post->Draw(dxCommon->GetCmdList());
	dxCommon->PostDraw();
}

void TitleScene::Finalize()
{
	delete title;
	delete dxCommon;

	//delete particle;
}