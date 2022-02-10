#include "MyGame.h"

void MyGame::Initialize()
{
	//���N���X�̏�����
	Framework::Initialize();

	gamescene->Initialize(dxCommon);
	gamescene->GameSceneInitialize();
}

void MyGame::Update()
{
	//���N���X�̍X�V
	Framework::Update();

	gamescene->Update(dxCommon, input);
}

void MyGame::Draw()
{
	//�`�揈��
	dxCommon->PreDraw();

	gamescene->Draw(dxCommon);
	dxCommon->PostDraw();
}

void MyGame::Finalize()
{
	//���N���X�̏I��
	Framework::Finalize();
	
	//delete contoroller;
	//delete model;
}
