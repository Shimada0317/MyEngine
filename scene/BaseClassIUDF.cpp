#include "BaseClassIUDF.h"
#include"GameScene.h"
#include"TitleScene.h"
#include"ClearScene.h"
#include"DebugScene.h"

void BaseClassIUDF::Initialize()
{
	//���N���X�̏�����
	Framework::Initialize();
	BaseScene* scene_ = new TitleScene(sceneManager_);
	//�V�[���}�l�[�W���[�ɍŏ��̃V�[�����Z�b�g
	sceneManager_->SetNextScene(scene_);

}

void BaseClassIUDF::Updata()
{
	//���N���X�̍X�V
	Framework::Updata();

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
