#pragma once

#include"GameScene.h"
#include"WinApp.h"
#include"DirectXCommon.h"
#include"Input.h"
#include"Audio.h"

/// <summary>
/// �t���[�����[�N
/// </summary>
class Framework
{
public:
	/// <summary>
	/// ���s
	/// </summary>
	void Run();


	/// <summary>
	/// ������
	/// </summary>
	virtual void Initialize();

	/// <summary>
	/// �I��
	/// </summary>
	virtual void Finalize();

	/// <summary>
		/// �X�V
		/// </summary>
	virtual void Update();
	/// <summary>
	/// �`��
	/// </summary>
	virtual void Draw() = 0;
	bool GetFinish() { return finish; }
protected:
	bool finish = false;

	DirectXCommon* dxCommon = nullptr;
	WinApp* winApp = nullptr;
	Input* input = nullptr;
	GameScene* gamescene = nullptr;

	Audio* audio = nullptr;
	
};

