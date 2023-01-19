#pragma once
#include"BaseScene.h"
#include"DirectXCommon.h"
/// <summary>
/// �V�[���Ǘ�
/// </summary>
class SceneManager
{
public:
	~SceneManager();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update(DirectXCommon* dxComon);

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(DirectXCommon* dxCommon);

	//���V�[���̗\��
	void SetNextScene(BaseScene* nextScene) { nextscene_ = nextScene; }
private:
	//���̃V�[��
	BaseScene* scene_ = nullptr;
	//���̃V�[��
	BaseScene* nextscene_ = nullptr;
};

