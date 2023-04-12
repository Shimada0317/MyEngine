#include "TitleScene.h"
#include<cassert>
#include <sstream>
#include <iomanip>
#include"Action.h"
#include"Mouse.h"


const float SubColor = 0.01f;
const float CameraMoveValueXandY = 0.4f;
const float CameraMoveValueZ = 0.1f;
const float CameraEyeMoveValue = 0.01f;
const XMFLOAT4 RegularColor = { 1.f,1.f,1.f,1.f };
const XMFLOAT2 DescriptionScreenPosition = { WinApp::window_width / 2.0f,WinApp::window_height / 2.0f - 72.0f };

//�R���X�g���N�^
TitleScene::TitleScene(SceneManager* sceneManager_)
	:BaseScene(sceneManager_)
{

}

//����������
void TitleScene::Initialize(DirectXCommon* dxComon)
{
	TitleCamera = make_unique<Camera>(WinApp::window_width, WinApp::window_height); 
	Object3d::SetCamera(TitleCamera.get());
	light = make_unique<Light>();
	light = Light::Create();

	lightGroupe = make_unique<LightGroup>();
	lightGroupe = LightGroup::Create();

	Object3d::SetLightGroup(lightGroupe.get());
	Object3d::SetLight(light.get());

	
	//�X�v���C�g�̐���
	Title.reset(Sprite::SpriteCreate(Name::kTitle, { 1.0f,1.0f }));
	Cursor.reset(Sprite::SpriteCreate(Name::kReticle, ReticlePos, SpriteCol, Anchorpoint));
	ClickBefore.reset(Sprite::SpriteCreate(3, ClickPos, RegularColor, Anchorpoint));
	ClickAfter.reset(Sprite::SpriteCreate(4, ClickPos, RegularColor, Anchorpoint));
	SignalBefore.reset(Sprite::SpriteCreate(5, ClickPos, RegularColor, Anchorpoint));
	SignalAfter.reset(Sprite::SpriteCreate(6, ClickPos, RegularColor, Anchorpoint));
	DescriptionOperation.reset(Sprite::SpriteCreate(7, DescriptionScreenPosition, SpriteCol, Anchorpoint));
	EnemyOverview.reset(Sprite::SpriteCreate(8, DescriptionScreenPosition, SpriteCol, Anchorpoint));
	GameStartPreparation.reset(Sprite::SpriteCreate(9, DescriptionScreenPosition, SpriteCol, Anchorpoint));
	ArrowRight.reset(Sprite::SpriteCreate(10, ArrowRightPos, ArrowRightColor, Anchorpoint));
	ArrowLeft.reset(Sprite::SpriteCreate(11, ArrowLeftPos, ArrowLeftColor, Anchorpoint));


	//�I�u�W�F�N�g�̐���
	Sphere = Object3d::Create(ModelManager::GetInstance()->GetModel(6));
	for (int i = 0; i < BILLS; i++) {
		BillsHighAlpha[i] = Object3d::Create(ModelManager::GetInstance()->GetModel(7));
		BillsLowAlpha[i] = Object3d::Create(ModelManager::GetInstance()->GetModel(7));
	}
	Start = Object3d::Create(ModelManager::GetInstance()->GetModel(8));
	World = Object3d::Create(ModelManager::GetInstance()->GetModel(9));
	//SE�̏�����
	ClickSe = make_unique<Audio>();
	ClickSe->Initialize();
	Bgm = make_unique<Audio>();
	Bgm->Initialize();
	Bgm->LoopWave("Resources/Sound/BGM/title.wav", 0.75f);
	//�|�X�g�G�t�F�N�g�̏�����
	Post = make_unique<PostEffect>();
	Post->Initialize();
	CameraEyeMove = { 0.0f,0.0f,0.0f };
	//�X�|�b�g���C�g���A�N�e�B�u���
	lightGroupe->SetSpotLightActive(0, true);
	lightGroupe->SetSpotLightActive(1, true);
	lightGroupe->SetSpotLightActive(2, true);
}

//�X�e�[�^�X�Z�b�g
void TitleScene::StatusSet()
{
	//�J�����̓���
	TitleCamera->MoveEyeVector(CameraEyeMove);
	TitleCamera->MoveVector(CameraMove);
	//�V���̃X�e�[�^�X�̃Z�b�g
	Sphere->SetRotation(SphereRot);
	Sphere->SetPosition(SpherePos);
	Sphere->SetScale(SphereScl);
	//�n�ʂ̃X�e�[�^�X�̃Z�b�g
	World->SetPosition(WorldPos);
	World->SetScale(WorldScl);
	//���E�̃r���̃X�e�[�^�X�̃Z�b�g
	for (int i = 0; i < BILLS; i++) {
		BillsHighAlpha[i]->SetScale(BillsScl);
		BillsLowAlpha[i]->SetScale(BillsScl);
		if (i % 2 == 0) {
			BillsHighAlphaPos = { 100.0f, 0,-300.0f + (100 * i / 2) };
			BillsLowAlphaPos = { 200.0f,0,-300.0f + (100 * i / 2) };
			BillsRot = { 0.0f,90.0f,0.0f };
		}
		else if (i % 2 == 1) {
			BillsHighAlphaPos = { -100.0f,0,-300.0f + (100 * i / 2) };
			BillsLowAlphaPos = { -200.0f, 0,-300.0f + (100 * i / 2) };
			BillsRot = { 0.0f,270.0f,0.0f };
		}
		BillsHighAlpha[i]->SetRotation(BillsRot);
		BillsHighAlpha[i]->SetPosition(BillsHighAlphaPos);
		BillsLowAlpha[i]->SetRotation(BillsRot);
		BillsLowAlpha[i]->SetPosition(BillsLowAlphaPos);
	}
	//�J�����̈ړ���̃r���̃X�e�[�^�X�Z�b�g
	Start->SetPosition(StartPos);
	Start->SetScale(StartScl);
	Start->SetRotation({ 0.0f,180.0f,0.0f });
	//�^�C�g���X�v���C�g�̃X�e�[�^�X�Z�b�g
	Title->SetSize({ TitleSize });
	Title->SetPosition({ TitlePos });
	//�}�E�X�J�[�\���̍��W�Z�b�g
	Cursor->SetPosition({ ReticlePos });

	//1�ڂ̃X�|�b�g���C�g��ݒ�
	lightGroupe->SetSpotLightDir(0, XMVECTOR({ SpotLightDir.x, SpotLightDir.y, SpotLightDir.z }));
	lightGroupe->SetSpotLightPos(0, SpotLightPos);
	lightGroupe->SetSpotLightColor(0, SpotLightColor);
	lightGroupe->SetSpotLightAtten(0, SpotLightAtten);
	lightGroupe->SetSpotLightFactorAngle(0, SpotLightFactorAngle);

	//2�ڂ̃X�|�b�g���C�g��ݒ�
	lightGroupe->SetSpotLightDir(1, XMVECTOR({ SpotLightDir2.x, SpotLightDir2.y, SpotLightDir2.z }));
	lightGroupe->SetSpotLightPos(1, SpotLightPos2);
	lightGroupe->SetSpotLightColor(1, SpotLightColor2);
	lightGroupe->SetSpotLightAtten(1, SpotLightAtten2);
	lightGroupe->SetSpotLightFactorAngle(1, SpotLightFactorAngle2);

	//1�ڂ̃X�|�b�g���C�g��ݒ�
	lightGroupe->SetSpotLightDir(2, XMVECTOR({ SpotLightDir.x, SpotLightDir.y, SpotLightDir.z }));
	lightGroupe->SetSpotLightPos(2, SpotLightPos);
	lightGroupe->SetSpotLightColor(2, SpotLightColor);
	lightGroupe->SetSpotLightAtten(2, SpotLightAtten);
	lightGroupe->SetSpotLightFactorAngle(2, SpotLightFactorAngle);


	ArrowLeft->SetSize(ArrowSize);
	ArrowRight->SetSize(ArrowSize);
	ArrowLeft->SetColor(ArrowLeftColor);
	ArrowRight->SetColor(ArrowRightColor);

	ClickAfter->SetSize(ClickSize);
	ClickBefore->SetSize(ClickSize);
	SignalAfter->SetSize(ClickSize);
	SignalBefore->SetSize(ClickSize);

}

//�S�Ă̍X�V�������܂Ƃ߂�
void TitleScene::AllUpdate()
{
	//���E�̃r���̍X�V����
	for (int i = 0; i < BILLS; i++) {
		BillsHighAlpha[i]->Update({ 0.8f,0.6f,0.3f,1.0f });
		BillsLowAlpha[i]->Update({ 0.2f,0.2f,0.2f,0.9f });
	}
	//�|�X�g�G�t�F�N�g�̍X�V����
	Post->Update(PostEfectColor);
	//�V���̍X�V����
	Sphere->Update({ 1,1,1,1 },true);
	//�n�ʂ̍X�V����
	World->Update();
	//�J�����̈ړ���̃r���̍X�V����
	Start->Update({ 0.4f,0.4f,0.4f,1.f });
	//���C�g�O���[�v�X�V
	lightGroupe->Update();
}

//�X�V����
void TitleScene::Update()
{
	if (EasingChangeFlag == false) {
		EasingTimer += 0.05f;
		if (EasingTimer >= 1) {
			EasingChangeFlag = true;
		}
	}
	else {
		EasingTimer -= 0.05f;
		if (EasingTimer <= -1) {
			EasingChangeFlag = false;
		}
	}
	ArrowSize.x = Action::GetInstance()->EasingOut(EasingTimer, 5) + 32;
	ArrowSize.y = Action::GetInstance()->EasingOut(EasingTimer, 5) + 32;
	ClickSize.x= Action::GetInstance()->EasingOut(EasingTimer, 5) + 550;
	ClickSize.y= Action::GetInstance()->EasingOut(EasingTimer, 5) + 60;
	Mouse::GetInstance()->MouseMoveSprite(ReticlePos);
	//�J�����̃��[�u�֐�
	CameraDirection();
	//�J�[�\�����X�v���C�g�͈͓̔��ł��邩
	CheckCursorIn(ReticlePos, ClickPos, 250.f, 50.f, SignalCurorInFlag);
	//�ŏ��̃N���b�N
	if (SignalCurorInFlag == true && TitleDisplay_F == true) {
		if (Mouse::GetInstance()->PushClick(0) || Mouse::GetInstance()->PushClick(1)) {
			TitleSprite_F = false;
			CameraEyeMove_F = true;
			TitleDisplay_F = false;
		}
	}

	DescriptionPageProces();

	FadeOutAndSceneChange();

	StatusSet();

	AllUpdate();
	//�J�����̍Čv�Z
	TitleCamera->RecalculationMatrix();
}

//�J�����̈ړ�
void TitleScene::CameraDirection()
{
	CameraMove = { 0.0f,0.0f,0.0f };
	if (CameraEyeMove_F == true && CameraChange_F == false) {
		if (CameraEyeMove.m128_f32[0] >= -1.1) {
			CameraEyeMove.m128_f32[0] -= CameraEyeMoveValue;
			CameraMove.m128_f32[0] += CameraMoveValueXandY;
			CameraMove.m128_f32[1] -= CameraMoveValueXandY;
			CameraMove.m128_f32[2] -= CameraMoveValueZ;
		}
		else {
			CameraChange_F = true;
		}
	}

	if (CameraChange_F == true) {
		CameraEyeMove = { 0.0f,0.0f,0.0f };
	}
}

//�J�[�\�����͈͓��ɓ����Ă��邩
void TitleScene::CheckCursorIn(const XMFLOAT2& cursor_Pos, const XMFLOAT2& check_Pos, float radX, float radY, bool& CheckFlag)
{
	if ((check_Pos.x - radX <= cursor_Pos.x && check_Pos.x + radX >= cursor_Pos.x) &&
		(check_Pos.y - radY <= cursor_Pos.y && check_Pos.y + radY >= cursor_Pos.y)) {
		CheckFlag = true;
	}
	else {
		CheckFlag = false;
	}
}

//���̃X�v���C�g�͈̔�
bool TitleScene::NextorBack(const XMFLOAT2& cursor_Pos, const XMFLOAT2& check_Pos, float radX, float radY)
{
	if ((check_Pos.x - radX <= cursor_Pos.x && check_Pos.x + radX >= cursor_Pos.x) &&
		(check_Pos.y - radY <= cursor_Pos.y && check_Pos.y + radY >= cursor_Pos.y)) {
		return true;
	}

	return false;
}


void TitleScene::DescriptionPageProces()
{
	//�J�������ړ�������̉��
	if (DescriptionPage < 2 && TitleDisplay_F == false && CameraChange_F == true) {
		if (NextorBack(ReticlePos, ArrowRightPos, 16, 16)) {
			ArrowRightColor = { 1.f,0.f,0.f,1.f };
			RightTrueIn_F = true;
			//���������ꂽ��
			if ((Mouse::GetInstance()->PushClick(0) || Mouse::GetInstance()->PushClick(1))) {
				ClickSe->LoadFile("Resources/Sound/SE/click.wav", Volume);
				DescriptionPage += 1;
			}
		}
		else {
			ArrowRightColor = { 1.f,1.f,1.f,1.f };
			RightTrueIn_F = false;
		}
	}

	//�y�[�W��0�ȏ�ł����
	if (DescriptionPage > 0) {
		if (NextorBack(ReticlePos, ArrowLeftPos, 16, 16)) {
			ArrowLeftColor = { 1.f,0.f,0.f,1.f };
			LeftTrueIn_F = true;
			if ((Mouse::GetInstance()->PushClick(0) || Mouse::GetInstance()->PushClick(1))) {
				ClickSe->LoadFile("Resources/Sound/SE/click.wav", Volume);
				DescriptionPage -= 1;
			}
		}
		else {
			ArrowLeftColor = { 1.f,1.f,1.f,1.f };
			LeftTrueIn_F = false;
		}
	}

	//�~���w�����ĂԂƂ�
	if (CameraChange_F == true && SignalCurorInFlag == true && DescriptionPage == 2) {
		if (Mouse::GetInstance()->PushClick(0) || Mouse::GetInstance()->PushClick(1)) {
			if (Click_F == true) {
				ClickSe->LoadFile("Resources/Sound/SE/MorseCode.wav", Volume);
				Click_F = false;
			}
			FadeOut_F = true;
		}
	}
}

//�t�F�[�h�A�E�g��ɃQ�[���V�[���փ`�F���W
void TitleScene::FadeOutAndSceneChange()
{
	//�~���w����ǂ񂾌�
	if (FadeOut_F == true) {
		PostEfectColor.x -= SubColor;
		PostEfectColor.y -= SubColor;
		PostEfectColor.z -= SubColor;
		PostEfectColor.w -= SubColor;
		if (PostEfectColor.w <= 0) {
			//�V�[���؂�ւ�
			BaseScene* scene_ = new GameScene(sceneManager_);
			sceneManager_->SetNextScene(scene_);
		}
	}
}

//�`�揈��
void TitleScene::Draw(DirectXCommon* dxCommon)
{
	Post->PreDrawScene(dxCommon->GetCmdList());

	Object3d::PreDraw(dxCommon->GetCmdList());
	World->Draw();
	Sphere->Draw();
	for (int i = 0; i < BILLS; i++) {
		BillsHighAlpha[i]->Draw();
		BillsLowAlpha[i]->Draw();
	}
	Start->Draw();
	Object3d::PostDraw();

	Sprite::PreDraw(dxCommon->GetCmdList());
	if (TitleSprite_F == true) {
		Title->Draw();
	}
	if (CameraEyeMove_F == false) {
		if (SignalCurorInFlag == false) {
			ClickBefore->Draw();
		}
		else if (SignalCurorInFlag == true) {
			ClickAfter->Draw();
		}
	}
	else if (CameraChange_F == true) {
		if (DescriptionPage < 2) {
			ArrowRight->Draw();
		}
		if (DescriptionPage > 0) {

			ArrowLeft->Draw();


		}
		if (DescriptionPage == 0) {
			DescriptionOperation->Draw();
		}
		else if (DescriptionPage == 1) {
			EnemyOverview->Draw();
		}
		else if (DescriptionPage == 2) {
			GameStartPreparation->Draw();
		}
		if (DescriptionPage == 2) {
			if (SignalCurorInFlag == false) {
				SignalBefore->Draw();
			}
			else if (SignalCurorInFlag == true) {
				SignalAfter->Draw();
			}
		}
	}
	Cursor->Draw();
	Sprite::PostDraw();
	Post->PostDrawScene(dxCommon->GetCmdList());

	dxCommon->PreDraw();
	Post->Draw(dxCommon->GetCmdList());
	dxCommon->PostDraw();
}

//�I������
void TitleScene::Finalize()
{

	Title.reset();
	ArrowLeft.reset();
	ArrowRight.reset();

	EnemyOverview.reset();
	DescriptionOperation.reset();
	GameStartPreparation.reset();
	SignalAfter.reset();
	SignalBefore.reset();
	Cursor.reset();
	for (int i = 0; i < BILLS; i++) {
		BillsHighAlpha[i].reset();
		BillsLowAlpha[i].reset();
	}
	Sphere.reset();
	//World.reset();
	Start.reset();
}