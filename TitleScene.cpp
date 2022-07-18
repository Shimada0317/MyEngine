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

	//モデルの読み込み
	/*playermodel = Model::LoadFromObJ("player");
	player3d = Object3d::Create();
	player3d->SetModel(playermodel);*/

	ramieru = ObjModel::CreateFromOBJ("ramieru");
	ramieru3d = Object3d::Create();
	ramieru3d->SetModel(ramieru);

	particle = ParticleManager::Create();
	particle->Update();
}

void TitleScene::SetPosSclRot()
{
	//プレイヤー
	/*player3d->SetRotation({ 0,450,-270 });
	player3d->SetPosition({ 0,0,0 });
	player3d->SetScale({0.5f,0.5f,0.5f});*/



	title->SetSize({ 1280.0f,720.0f });

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
	ramieru3d->Update();
	particle->Update();
}

void TitleScene::Draw(DirectXCommon* dxCommon)
{
	dxCommon->PreDraw();
	// コマンドリストの取得
	//ID3D12GraphicsCommandList* cmdList = dxCommon->GetCmdList();

	//dxCommon->GetCmdList()->IASetVertexBuffers(0, 1, &vbView);

	Sprite::PreDraw(dxCommon->GetCmdList());
	title->Draw();
	Sprite::PostDraw();
	ParticleManager::PreDraw(dxCommon->GetCmdList());
	//particle->Draw();
	ParticleManager::PostDraw();
	dxCommon->PostDraw();
}

void TitleScene::Finalize()
{
	delete title;
	delete player3d;
	delete dxCommon;
	delete ramieru3d;
	delete ramieru;
	//delete particle;
}