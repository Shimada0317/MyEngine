#pragma once

#pragma once

#include"Audio.h"
#include"BaseScene.h"
#include "DirectXCommon.h"
#include <DirectXMath.h>
#include "Sprite.h"
#include"Object3d.h"
#include"ObjModel.h"
#include"ParticleManager.h"
#include"PostEffect.h"
#include<memory>
#include"Light.h"
#include"Camera.h"

using namespace std;

const int BILL = 16;

class TitleScene :public BaseScene
{
private: // �G�C���A�X
// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;
private:
	static const int debugTextNumber = 0;

public://�����o�֐�
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="sceneManager_"></param>
	TitleScene(SceneManager* sceneManager_);

	/// <summary>
	/// ����������
	/// </summary>
	/// <param name="dxComon"></param>
	void Initialize(DirectXCommon* dxComon) override;

	/// <summary>
	/// �X�e�[�^�X�Z�b�g
	/// </summary>
	void StatusSet();

	/// <summary>
	/// Obj����Update�͂��̒���
	/// </summary>
	void AllUpdate();

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update() override;

	/// <summary>
	/// �J�����̈ړ�
	/// </summary>
	void CameraDirection();

	/// <summary>
	/// �J�[�\�����͈͓��ɓ����Ă��邩
	/// </summary>
	/// <param name="cursorPos">�J�[�\���̍��W</param>
	/// <param name="checkPos">������X�v���C�g�̍��W</param>
	/// <param name="radX">�X�v���C�g�̉��͈̔�</param>
	/// <param name="radY">�X�v���C�g�̏c�͈̔�</param>
	void CheckCursorIn(const XMFLOAT2& cursorPos, const XMFLOAT2& checkPos, float radX, float radY);

	/// <summary>
	/// ���̃X�v���C�g�͈̔�
	/// </summary>
	/// <param name="cursorPos">�J�[�\���̍��W</param>
	/// <param name="checkPos">������X�v���C�g�̍��W</param>
	/// <param name="radX">�X�v���C�g�̉��͈̔�</param>
	/// <param name="radY">�X�v���C�g�̏c�͈̔�</param>
	/// <returns></returns>
	bool NextorBack(const XMFLOAT2& cursorPos, const XMFLOAT2& checkPos, float radX, float radY);

	/// <summary>
	/// ������ʂōs���鏈��
	/// </summary>
	void DescriptionPageOperation();

	/// <summary>
	///�@�t�F�[�h�A�E�g��ɃQ�[���V�[���փ`�F���W
	/// </summary>
	void FadeOutAndSceneChange();

	/// <summary>
	/// �`�揈��
	/// </summary>
	/// <param name="dxCommon"></param>
	void Draw(DirectXCommon* dxCommon)override;

	/// <summary>
	/// �I������
	/// </summary>
	void Finalize() override;

private:
	//Obj
	unique_ptr <Object3d> Sphere;
	unique_ptr<Object3d> BillsHighAlpha[BILL];
	unique_ptr<Object3d> BillsLowAlpha[BILL];
	unique_ptr <Object3d> GroundObj;
	unique_ptr <Object3d >World;
	unique_ptr<Object3d> Start;
	//�X�v���C�g
	Sprite* Title = nullptr;
	Sprite* Cursor = nullptr;
	Sprite* ClickBefore = nullptr;
	Sprite* ClickAfter = nullptr;
	Sprite* SignalBefore = nullptr;
	Sprite* SignalAfter = nullptr;
	Sprite* DescriptionOperation = nullptr;
	Sprite* EnemyOverview = nullptr;
	Sprite* GameStartPreparation = nullptr;
	Sprite* ArrowRight = nullptr;
	Sprite* ArrowLeft = nullptr;
	Sprite* ArrowRightTrue = nullptr;
	Sprite* ArrowLeftTrue = nullptr;
	//���̑��̋@�\
	Light* light = nullptr;
	PostEffect* Post = nullptr;
	Audio* ClickSe = nullptr;
	Audio* MorseCodeSe = nullptr;
	Audio* Bgm = nullptr;
	Camera* TitleCamera = nullptr;
	DirectXCommon* dxCommon = nullptr;
	//���̂̃X�e�[�^�X
	XMVECTOR SpherePos = { 0,0,0 };
	XMFLOAT3 SphereScl = { 4.0f,4.0f,4.0f };
	XMFLOAT3 SphereRot = { 0,0,0 };
	//�t�B�[���h�̃X�e�[�^�X
	XMVECTOR GroundPos = { 22.5f,-1,40 };
	XMFLOAT3 GroundScl = { 13,20,16 };
	XMFLOAT3 GroundRot = { 0,0,0 };
	//�r���Q�̃X�e�[�^�X
	XMFLOAT3 BillsScl = { 10.0f,10.0f,10.0f };
	XMVECTOR BillsHighAlphaPos = { 0.0f,0.0f,-16.5f };
	XMVECTOR BillsLowAlphaPos = { 0.0f,0.0f,-16.5f };
	XMFLOAT3 BillsRot = { 0.0f,90.0f,0.0f };
	//�J�����̈ړ���̃X�e�[�^�X
	XMVECTOR StartPos = { 0.0f,0.0f,-16.5f };
	XMFLOAT3 StartScl = { 15.0f,15.0f,15.0f };
	//�J�����̃X�e�[�^�X
	XMVECTOR CameraMove = { 0.0f,0.0f,0.0f };
	XMVECTOR CameraEyeMove = { 0.0f,0.0f,0.0f };
	bool CameraEyeMove_F = false;
	bool CameraChange_F = false;
	//����̃X�e�[�^�X
	XMVECTOR WorldPos = { 0.0f,-1.1f,0.0f };
	XMFLOAT3 WorldScl = { 100,100,100 };
	//�X�v���C�g�̃X�e�[�^�X
	XMFLOAT4 SpriteCol = { 1,1,1,1 };
	XMFLOAT2 Anchorpoint = { 0.5f,0.5f };
	XMFLOAT2 ReticlePos = { 640.0f,360.0f };
	XMFLOAT2 ClickPos = { 380,560 };
	//�^�C�g���X�v���C�g�̃X�e�[�^�X
	XMFLOAT2 TitleSize = { 1280.0f,720.0f };
	XMFLOAT2 TitlePos = { 0.0f,0.0f };
	bool TitleSprite_F = true;
	//�����̖����W
	XMFLOAT2 ArrowRightPos = { 1220.0f,300 };
	XMFLOAT2 ArrowLeftPos = { 30.0f,300 };
	bool RightTrueIn_F = false;
	bool LeftTrueIn_F = false;
	//�|�X�g�G�t�F�N�g�̐F
	XMFLOAT4 PostEfectColor = { 0.0f,0.0f,0.0f,1.0f };
	//�J�[�\���������Ă��邩
	bool CursorIn_F = false;
	//�t�F�[�h���|���邩�|���Ȃ���
	bool FadeOut_F = false;
	bool TitleDisplay_F = true;
	//�N���b�N���Ă��邩�ǂ���
	bool Click_F = true;
	//����
	float Volume = 0.8f;
	//�����y�[�W
	int DescriptionPage = 0;
};

