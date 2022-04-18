#include "SceneManager.h"

SceneManager::~SceneManager()
{
	//�Ō�̃V�[���̏I���Ɖ��
	scene_->Finalize();
	delete scene_;
}

void SceneManager::Update()
{
	//�V�[���̐؂�ւ�������Ȃ�
	if (nextscene_) {
		//���V�[���̏I��
		if (scene_) {
			scene_->Finalize();
			delete scene_;
		}
		//�V�[���؂�ւ�
		scene_ = nextscene_;
		nextscene_ = nullptr;

		scene_->Initialize();
	}

	scene_->Update();
}

void SceneManager::Draw(DirectXCommon* dxCommon)
{
	scene_->Draw(dxCommon);
}