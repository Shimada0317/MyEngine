#include "MyGame.h"
#include"GameScene.h"

void MyGame::Initialize()
{
	//���N���X�̏�����
	Framework::Initialize();
	scene_ = new GameScene();
	scene_->Initialize(dxCommon);
	scene_->GameSceneInitialize();
}

void MyGame::Update()
{
	//���N���X�̍X�V
	Framework::Update();

	scene_->Update(dxCommon, input);
}

void MyGame::Draw()
{
	//�`�揈��
	dxCommon->PreDraw();

	scene_->Draw(dxCommon);
	dxCommon->PostDraw();
}

void MyGame::Finalize()
{
	//���N���X�̏I��
	Framework::Finalize();
	scene_->Finalize();
	//delete contoroller;
	//delete model;
}
