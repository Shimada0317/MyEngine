#include "BaseClassIUDF.h"
#include"GameScene.h"
#include"TitleScene.h"
#include"ClearScene.h"
#include"DebugScene.h"

void BaseClassIUDF::Initialize()
{
	//���N���X�̏�����
	Framework::Initialize();
	BaseScene* scene_ = new DebugScene(sceneManager_);
	//�V�[���}�l�[�W���[�ɍŏ��̃V�[�����Z�b�g
	sceneManager_->SetNextScene(scene_);

}

void BaseClassIUDF::Update()
{
	//���N���X�̍X�V
	Framework::Update();

}

void BaseClassIUDF::Draw()
{
	Framework::Draw();
}

void BaseClassIUDF::Finalize()
{
	//���N���X�̏I��
	Framework::Finalize();

}
