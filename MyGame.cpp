#include "MyGame.h"
#include"GameScene.h"
#include"TitleScene.h"

void MyGame::Initialize()
{
	//���N���X�̏�����
	Framework::Initialize();
	BaseScene* scene_ = new TitleScene(sceneManager_);
	//�V�[���}�l�[�W���[�ɍŏ��̃V�[�����Z�b�g
	sceneManager_->SetNextScene(scene_);

}

void MyGame::Update()
{
	//���N���X�̍X�V
	Framework::Update();

}

void MyGame::Draw()
{
	Framework::Draw();
}

void MyGame::Finalize()
{
	//���N���X�̏I��
	Framework::Finalize();

}
