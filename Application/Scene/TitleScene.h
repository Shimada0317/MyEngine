#pragma once
#include <DirectXMath.h>
#include<memory>
#include"Audio.h"
#include"BaseScene.h"
#include"Camera.h"
#include "DirectXCommon.h"
#include"GameScene.h"
#include"Light.h"
#include"LightGroup.h"
#include"Object3d.h"
#include"ObjModel.h"
#include"SceneManager.h"
#include"ParticleManager.h"
#include"PostEffect.h"
#include "Sprite.h"

using namespace std;

const int BUILAMOUNT = 16;

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
	/// <param name="CheckFlag">�͈͓��ɓ����Ă���ꍇ�t���O��true�ɂ���</param>
	void CheckCursorIn(const XMFLOAT2& cursorPos, const XMFLOAT2& checkPos, float radX, float radY,bool& CheckFlag);

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
	void DescriptionPageProces();

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
	unique_ptr<Object3d> BillsHighAlpha[BUILAMOUNT];
	unique_ptr<Object3d> BillsLowAlpha[BUILAMOUNT];
	unique_ptr <Object3d> GroundObj;
	unique_ptr <Object3d >World;
	unique_ptr<Object3d> Start;
	//�X�v���C�g
	unique_ptr<Sprite> Title = nullptr;
	unique_ptr<Sprite> Cursor = nullptr;
	unique_ptr<Sprite> ClickBefore = nullptr;
	unique_ptr<Sprite> ClickAfter = nullptr;
	unique_ptr<Sprite> SignalBefore = nullptr;
	unique_ptr<Sprite> SignalAfter = nullptr;
	unique_ptr<Sprite> DescriptionOperation = nullptr;
	unique_ptr<Sprite> EnemyOverview = nullptr;
	unique_ptr<Sprite> GameStartPreparation = nullptr;
	unique_ptr<Sprite> ArrowRight = nullptr;
	unique_ptr<Sprite> ArrowLeft = nullptr;
	unique_ptr<Sprite> ArrowRightTrue = nullptr;
	unique_ptr<Sprite> ArrowLeftTrue = nullptr;
	//���̑��̋@�\
	unique_ptr<Light> light = nullptr;
	unique_ptr<LightGroup> lightGroupe = nullptr;
	unique_ptr<PostEffect> Post = nullptr;
	unique_ptr<Audio> ClickSe = nullptr;
	unique_ptr<Audio> MorseCodeSe = nullptr;
	unique_ptr<Audio> Bgm = nullptr;
	unique_ptr<Camera> TitleCamera = nullptr;
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
	XMFLOAT2 ClickPos = { WinApp::window_width/2,600 };
	//�^�C�g���X�v���C�g�̃X�e�[�^�X
	XMFLOAT2 TitleSize = { 1280.0f,720.0f };
	XMFLOAT2 TitlePos = { 0.0f,0.0f };
	bool TitleSprite_F = true;
	//�����̖����W
	XMFLOAT2 ArrowRightPos = { 1240.0f,300 };
	XMFLOAT2 ArrowLeftPos = { 35.0f,300 };
	XMFLOAT2 ArrowSize = { 32.f,32.f};
	XMFLOAT4 ArrowRightColor = { 1.f,1.f,1.f,1.f };
	XMFLOAT4 ArrowLeftColor = { 1.f,1.f,1.f,1.f };
	bool RightTrueIn_F = false;
	bool LeftTrueIn_F = false;
	//�|�X�g�G�t�F�N�g�̐F
	XMFLOAT4 PostEfectColor = { 0.0f,0.0f,0.0f,1.0f };
	//�J�[�\���������Ă��邩
	bool SignalCurorInFlag = false;
	//�t�F�[�h���|���邩�|���Ȃ���
	bool FadeOut_F = false;
	bool TitleDisplay_F = true;
	//�N���b�N���Ă��邩�ǂ���
	bool Click_F = true;
	//����
	float Volume = 0.8f;
	//�����y�[�W
	int DescriptionPage = 0;

	float PointLightPos[3] = { 0.f,0.f,0.f };
	float PointLightColor[3] = { 1.f,1.f,1.f };
	float PointLightAtten[3] = { 0.3f,0.1f,0.1f };


	XMFLOAT3 SpotLightDir = { 0,-1,0 };
	XMFLOAT3 SpotLightPos = { 0,1005,0 };
	XMFLOAT3 SpotLightColor = { 1,1,1 };
	XMFLOAT3 SpotLightAtten = { 0.f,0.f,0.f };
	XMFLOAT2 SpotLightFactorAngle = { 20.0f,30.0f };

	XMFLOAT3 SpotLightDir2 = { 0,0,0 };
	XMFLOAT3 SpotLightPos2 = { 10,1005,160 };
	XMFLOAT3 SpotLightColor2 = { 0,0,1 };
	XMFLOAT3 SpotLightAtten2 = { 0.f,0.f,0.f };
	XMFLOAT2 SpotLightFactorAngle2 = { 20.0f,30.0f };

	float EasingTimer = 0.f;
	bool EasingChangeFlag=false;
	XMFLOAT2 ClickSize = { 550.f,60.f };
};

