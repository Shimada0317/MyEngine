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
	////�X�v���C�g�̓ǂݍ���
	Sprite::LoadTexture(1, L"Resources/tst.png");
	title = Sprite::SpriteCreate(1, { 1.0f,1.0f });

	//���f���̓ǂݍ���
	/*playermodel = Model::LoadFromObJ("player");
	player3d = Object3d::Create();
	player3d->SetModel(playermodel);*/

	ramieru = Model::LoadFromObJ("ramieru");
	ramieru3d = Object3d::Create();
	ramieru3d->SetModel(ramieru);
}

void TitleScene::SetPosSclRot()
{
	//�v���C���[
	/*player3d->SetRotation({ 0,450,-270 });
	player3d->SetPosition({ 0,0,0 });
	player3d->SetScale({0.5f,0.5f,0.5f});*/



	title->SetSize({ 1280.0f,720.0f });
}

void TitleScene::Update()
{
	//DirectX���t���[������ ��������


	if (Input::GetInstance()->TriggerKey(DIK_RETURN)) {
		//�V�[���؂�ւ�
		BaseScene* scene_ = new GameScene(sceneManager_);
		sceneManager_->SetNextScene(scene_);
	}
	SetPosSclRot();
	ramieru3d->Update();

}

void TitleScene::Draw(DirectXCommon* dxCommon)
{
	dxCommon->PreDraw();
	// �R�}���h���X�g�̎擾
	//ID3D12GraphicsCommandList* cmdList = dxCommon->GetCmdList();

	//dxCommon->GetCmdList()->IASetVertexBuffers(0, 1, &vbView);
	Sprite::PreDraw(dxCommon->GetCmdList());
	title->Draw();
	Sprite::PostDraw();
	dxCommon->PostDraw();
}

void TitleScene::Finalize()
{
	delete title;
	delete player3d;
	delete dxCommon;
}