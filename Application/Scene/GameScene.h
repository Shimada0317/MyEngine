#pragma once
#include <DirectXMath.h>
#include "DirectXCommon.h"
#include"Actor.h"
#include"Audio.h"
#include"BaseScene.h"
#include"DebugCamera.h"
#include"Light.h"
#include"Object3d.h"
#include"ObjModel.h"
#include"PostEffect.h"
#include"ParticleManager.h"
#include"TitleScene.h"
#include "Sprite.h"

#include<memory>

using namespace std;

const int BILLS = 16;


class GameScene :public BaseScene
{
private: // �G�C���A�X
// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
private:


public://�����o�֐�
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="sceneManager_"></param>
	GameScene(SceneManager* sceneManager_);
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
	/// �I�u�W�F�N�g���̑S�Ă̍X�V����
	/// </summary>
	void AllUpdata();
	/// <summary>
	/// �X�V����
	/// </summary>
	void Update() override;
	/// <summary>
	/// �I�u�W�F�N�g�̕`��
	/// </summary>
	/// <param name="dxCommon">dxCommon����R�}���h���X�g���Q�b�g</param>
	void ObjDraw(DirectXCommon* dxCommon);
	/// <summary>
	/// �X�v���C�g�̕`��
	/// </summary>
	/// <param name="dxCommon">dxCommon����R�}���h���X�g���Q�b�g</param>
	void SpriteDraw(DirectXCommon* dxCommon);
	/// <summary>
	/// ImgUI�`��
	/// </summary>
	void ImgDraw();
	/// <summary>
	/// �|�X�g�G�t�F�N�g�̕`��
	/// </summary>
	/// <param name="dxCommon">dxCommon����R�}���h���X�g���Q�b�g</param>
	void PostEffectDraw(DirectXCommon* dxCommon);
	/// <summary>
	/// �`�揈��
	/// </summary>
	/// <param name="dxCommon">dxCommon����R�}���h���X�g���Q�b�g</param>
	void Draw(DirectXCommon* dxCommon)override;
	/// <summary>
	/// �I������
	/// </summary>
	void Finalize() override;
	/// <summary>
	/// �X�|�b�g���C�g�̓���
	/// </summary>
	void SpotLightMove();
	/// <summary>
	/// �Q�[���I�[�o�[���̏���
	/// </summary>
	void GameOverProcess();

	void CheckcCursorIn(const XMFLOAT2& cursor_Pos, const XMFLOAT2& check_Pos, float radX, float radY, bool& CheckFlag);
private://�����o�ϐ�
	//Obj
	unique_ptr <Object3d> Sphere;
	unique_ptr <Object3d> World;
	unique_ptr<Object3d> Start;
	unique_ptr<Object3d> BillsHighAlpha[BILLS];
	unique_ptr<Object3d> BillsLowAlpha[BILLS];
	unique_ptr<Object3d> FieldBills[5];
	//�X�v���C�g
	unique_ptr<Sprite> Clear = nullptr;
	unique_ptr<Sprite> Conteniu = nullptr;
	unique_ptr<Sprite> DamageEfectSp = nullptr;
	unique_ptr<Sprite> Shot = nullptr;
	unique_ptr<Sprite> ReticleForGameOver = nullptr;
	unique_ptr<Sprite> Yes = nullptr;
	unique_ptr<Sprite> No = nullptr;
	//���̑��̋@�\
	Audio* Bgm = nullptr;
	PostEffect* postEffect = nullptr;
	unique_ptr<Light> light = nullptr;
	unique_ptr<LightGroup> lightGroupe = nullptr;
	unique_ptr <Actor> Act = nullptr;
	//�ŏ��̃r���̃X�e�[�^�X
	XMVECTOR StartPos = { 0.0f,0.0f,-16.5f };
	XMFLOAT3 StartScl = { 15.0f,15.0f,15.0f };
	XMFLOAT3 StartRot = { 0.0f,180.0f,0.0f };
	//�V���̃X�e�[�^�X
	XMVECTOR SpherePos = { 0,0,0 };
	XMFLOAT3 SphereScl = { 4.0f,4.0f,4.0f };
	XMFLOAT3 SphereRot = { 0,90,0 };
	//�t�B�[���h�̃X�e�[�^�X
	XMFLOAT3 FieldBillScl = { 6.0f,10.0f,7.0f };
	XMVECTOR FieldBillPos[5];
	XMFLOAT3 FieldBillRot[5];
	//����̃X�e�[�^�X
	XMVECTOR WorldPos = { 0.0f,-1.1f,0.0f };
	XMFLOAT3 WorldScl = { 100,100,100 };
	//�r���Q�̃X�e�[�^�X
	XMFLOAT3 BillsScl = { 10.0f,10.0f,10.0f };
	XMVECTOR BillsHighAlphaPos = { 0.0f,0.0f,-16.5f };
	XMVECTOR BillsLowAlphaPos = { 0.0f,0.0f,-16.5f };
	XMFLOAT3 BillsRot = { 0.0f,90.0f,0.0f };
	//���̑��̕ϐ�
	XMFLOAT4 PostCol = { -1.0f,-1.0f,-1.0f,1.0f };
	int Patern = 0;
	int PlayerHp;
	int OldHp;
	bool PostEffectOnFlag = false;
	bool ClearFlag = false;
	bool StopUpdateFlag = false;
	bool DethFlag = false;
	bool GameStartFlag = false;
	bool DamageHitFlag = false;
	XMFLOAT4 DamageEfectColor = { 1.f,1.f,1.f,1.f };


	XMFLOAT3 FieldSpotLightDir = { 0,-10,0 };
	XMFLOAT3 FieldSpotLightPos = { 0,505,50 };
	XMFLOAT3 FieldSpotLightColor = { 0.9f,0.5f,0.f };
	XMFLOAT3 FieldSpotLightAtten = { 0.f,0.f,0.f };
	XMFLOAT2 FieldSpotLightFactorAngle = { 20.0f,30.0f };

	XMFLOAT3 PlayerSpotLightDir = { 0,-20,0 };
	XMFLOAT3 PlayerSpotLightPos = { 0,35,0 };
	XMFLOAT3 PlayerSpotLightColor = { 1.f,1.f,1.f };
	XMFLOAT3 PlayerSpotLightAtten = { 0.f,0.f,0.f };
	XMFLOAT2 PlayerSpotLightFactorAngle = { 20.0f,30.0f };

	XMFLOAT3 SearchLightDir[3];
	XMFLOAT3 SearchLightPos[3];
	XMFLOAT3 SearchLightColor[3];
	XMFLOAT3 SearchLightAtten = { 0.f,0.f,0.f };
	XMFLOAT2 SearchLightFactorAngle = { 20.0f,30.0f };

	bool SpotLightPositionChange = false;
	bool LightPositionChangeX[2];
	bool LightPositionChangeZ[2];

	float duration = 1.f;
	float time = -1.0f;
	float value = 0.f;
	float StartPointZ = 50.f;
	float EndPointZ = 0.f;
	float StartPointX = -30;
	float EndPointX = 30;
	float EbdPointZ2 = 90.0f;
	float StartPointZ2 = 50.f;

	bool Easing = true;
	float EasingWaitTimeR = 0.f;
	bool EasingChange = false;

	XMFLOAT3 SearchLightAddPos = { 0.1f,-0.1f,0.05f };
	float LightAddPosChangeTimer = 0.0f;
	bool ChangeTimerFlag = false;

	float StartColor = -0.5f;
	float EndColor = 0.0f;
	float ColorTime = 1.0f;

	float StartColorRed = 0.0f;
	float EndColorRed = 0.8f;
	float ColorTimeRed = 0.0f;

	float LightDirEasingTime = 0.f;
	bool LightDirEasingChane = false;

	XMFLOAT2 ReticlePosition{ 0.f,0.f };
	XMFLOAT4 ReticleColor{ 1.f,1.f,1.f,1.f };
	XMFLOAT2 SpriteAnchorPoint{ 0.5f,0.5f };

	XMFLOAT2 YesPosition{ WinApp::window_width / 2 - 200,WinApp::window_height / 2 + 100 };
	XMFLOAT2 NoPosition{ WinApp::window_width / 2 + 200,WinApp::window_height / 2 + 100 };
	XMFLOAT2 SpriteSize{ 240.f,240.f };
	XMFLOAT4 YesColor{ 1.f,1.f,1.f,1.f };
	XMFLOAT4 NoColor{ 1.f,1.f,1.f,1.f };
	bool YesCursorInFlag = false;
	bool NoCursorInFlag = false;
};

