#include "BaseClassIUDF.h"


void BaseClassIUDF::Initialize()
{
	//���N���X�̏�����
	Framework::Initialize();
	BaseScene* scene_ = new TitleScene(sceneManager_);
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
