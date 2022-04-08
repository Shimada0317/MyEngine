#include "GameScene.h"
#include<cassert>
#include <sstream>
#include <iomanip>
#include"SceneManager.h"

using namespace DirectX;

GameScene::GameScene(SceneManager* sceneManager_)
	:BaseScene(sceneManager_)
{

}

void GameScene::Initialize()
{
	////�X�v���C�g�̓ǂݍ���
	Sprite::LoadTexture(1, L"Resources/tst1.png");
	title = Sprite::SpriteCreate(1, { 1.0f,1.0f });

	//���f���̓ǂݍ���
	/*playermodel = Model::LoadFromObJ("player");
	player3d = Object3d::Create();
	player3d->SetModel(playermodel);*/

	ramieru = Model::LoadFromObJ("ramieru");
	ramieru3d = Object3d::Create();
	ramieru3d->SetModel(ramieru);
}

void GameScene::SetPosSclRot()
{
	//�v���C���[
	/*player3d->SetRotation({ 0,450,-270 });
	player3d->SetPosition({ 0,0,0 });
	player3d->SetScale({0.5f,0.5f,0.5f});*/

	ramieru3d->SetRotation({ 0,0,0 });
	ramieru3d->SetPosition({ ramieru_pos });
	ramieru3d->SetScale({ 1.0f,1.0f,1.0f });

	title->SetSize({ 1280.0f,720.0f });
}

void GameScene::Update()
{
	//DirectX���t���[������ ��������
	

	if (Input::GetInstance()->PushKey(DIK_1)) {
		
	}
	SetPosSclRot();
	ramieru3d->Update();

}

void GameScene::Draw(DirectXCommon* dxCommon)
{
	

	Sprite::PreDraw(dxCommon->GetCmdList());
	title->Draw();
	Sprite::PostDraw();

	//�I�u�W�F�N�g�O����
	Object3d::PreDraw();
	//player3d->Draw();
	ramieru3d->Draw();
	//human3d->Draw();
	//�I�u�W�F�N�g�㏈��
	Object3d::PostDraw();

}

void GameScene::Finalize()
{
	delete title;
	delete player3d;
}