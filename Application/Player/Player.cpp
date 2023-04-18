#include "Player.h"
#include<cassert>
#include "imgui.h"
#include"imconfig.h"
#include"Action.h"
#include"ModelManager.h"
#include"Mouse.h"
#include"WinApp.h"

#include"SpriteManager.h"

const int RemainingBullet = 8;
const float Gravity = 9.8f;

//�f�X�g���N�^
Player::~Player()
{
	delete PartGreen;
	delete PartRed;
	delete PartSmoke;
	delete ShotSe;
	delete ReloadSe;

	Gun.reset();
	Body.reset();
}

//����������
void Player::Initalize(Camera* camera)
{
	Object3d::SetCamera(camera);

	SkipPos = CurtainDownPos;

	//�X�v���C�g�̓ǂݍ���
	SpriteReticle.reset(Sprite::SpriteCreate(Name::kReticle, ReticlePos2D, ReticleColor, ReticleAncorPoint));
	CurtainUp.reset(Sprite::SpriteCreate(Name::kCurtain, CurtainUpPos));
	CurtainDown.reset(Sprite::SpriteCreate(Name::kCurtain, CurtainDownPos));
	Skip.reset(Sprite::SpriteCreate(Name::kSkip, SkipPos));
	for (int i = 0; i < 9; i++) {
		SpritePos[i] = { 1220.0f,25.0f + 32.0f * i };
		SpriteRot[i] = 0;
		Time[i] = 0;
		bulletHUD[i].reset(Sprite::SpriteCreate(Name::kBullet, SpritePos[i], { 1.0f,1.0f,1.0f,1.0f }, AnchorPoint));
		DropBulletFlag[i] = false;
	}
	Reload.reset(Sprite::SpriteCreate(Name::kReload, ReloadSpritePos, ReloadSpriteColor, AnchorPoint));

	Gun = Object3d::Create(ModelManager::GetInstance()->GetModel(10));
	Body = Object3d::Create(ModelManager::GetInstance()->GetModel(10));

	CurtainUp->SetSize(CurtainSize);
	CurtainDown->SetSize(CurtainSize);

	RailCam = new RailCamera();
	//	RailCam->MatrixIdentity(ReticlePos, ReticleRot);

	Body->SetParent(camera);

	PartGreen = ParticleManager::Create(camera);
	PartRed = ParticleManager::Create(camera);
	PartSmoke = ParticleManager::Create(camera);
	OldHp = Hp;

	ShotSe = new Audio();
	ShotSe->Initialize();

	ReloadSe = new Audio();
	ReloadSe->Initialize();
};

//�X�e�[�^�X�Z�b�g
void Player::StatusSet(Camera* camera)
{

	for (int i = 0; i < 8; i++) {
		if (DropBulletFlag[i] == true && SpritePos[i].y <= 1300) {
			SpritePos[i].y += 10;
			Time[i] += 0.5f;
			SpritePos[i].x += Action::GetInstance()->GetRangRand(-10, 10);
			Action::GetInstance()->ThrowUp(Gravity, Time[i], 40, SpritePos[i].y);
			SpriteRot[i] += 80;
		}
		else if (SpritePos[i].y > 1300) {
			Time[i] = 0;
		}
	}

	if (OldRemaining < Remaining) {
		DropBulletFlag[OldRemaining] = true;
		OldRemaining = Remaining;
	}

	BodyMat = Body->GetMatrix();
	BodyWorldPos = { -10.0f,0.0f,-20.0f };
	BodyWorldPos = XMVector3Transform(BodyWorldPos, BodyMat);

	ReticleRot.y = (ReticlePos2D.x - 640) / 10 + ChangeRot;

	Body->SetRotation(BodyRot);
	Body->SetPosition(BodyPos);
	Body->SetScale(BodyScl);
	Body->SetParent(camera);

	GunMat = Gun->GetMatrix();
	GunWorldPos = XMVector3Transform(GunPos, GunMat);

	Gun->SetRotation(GunRot);
	Gun->SetScale(GunScl);
	Gun->SetParent(camera);
	XMMATRIX GunNotParentMatrix = Gun->GetNotParentWorld();

	GunNotParentPos = XMVector3Transform(GunPos, GunNotParentMatrix);
	Gun->SetPosition(GunPos);

	//HUD�̃|�W�V�����Z�b�g
	for (int i = 0; i < 9; i++) {
		bulletHUD[i]->SetSize({ SpriteSiz });
		bulletHUD[i]->SetPosition(SpritePos[i]);
		bulletHUD[i]->SetRotation(SpriteRot[i]);
	}


	//�����[�h�̕���
	Reload->SetSize(ReloadSpriteSize);
	Reload->SetColor(ReloadSpriteColor);
}


void Player::AllUpdate()
{
	//RailCam->Update(Velocity, EyeRot, camera);
	Body->Update();
	Gun->Update();
	PartRed->Update({ 1.0f,0.0f,0.0f,0.0f });
	PartGreen->Update({ 0.0f,0.5f,0,0.0f });
	PartSmoke->Update({ 0.1f,0.1f,0.1f,0.0f });
}


//�X�V����
void Player::Update(Camera* camera, Phase patern, float changerotbool)
{
	DamageProcess();

	MouseContoroll();

	WaitProcess();

	GunShotProcess(patern);

	UIMotionProcess();

	ScreenShake(ShakingValue, 0.1f);

	ReloadProcess();

	Velocity = XMVector3TransformNormal(Velocity, BodyMat);

	StatusSet(camera);

	AllUpdate();
}

void Player::WaitProcess()
{
	if (playerstate_ == WAIT) {
		GunRot.x = (ReticlePos2D.y - WinApp::window_height / 2) / 50;
	}
}

//�p�[�e�B�N���`��
void Player::ParticleDraw(ID3D12GraphicsCommandList* cmdeList)
{
	ParticleManager::PreDraw(cmdeList);
	PartSmoke->Draw();
	PartRed->Draw();
	PartGreen->Draw();
	ParticleManager::PostDraw();
}

//�X�v���C�g�`��
void Player::SpriteDraw()
{
	if (MouseStopFlag == false) {
		for (int i = 0; i < 8; i++) {
			if (Remaining <= 8 && ReloadFlag == false) {
				bulletHUD[i]->Draw();
			}
		}
		if (Remaining >= 8) {
			Reload->Draw();
		}
	}
	SpriteReticle->Draw();
}


//�I�u�W�F�N�g�`��
void Player::ObjDraw()
{
	if (Hp >= 0) {
		Gun->Draw();
	}
}

//ImgUi�`��
void Player::ImGuiDraw()
{

	XMMATRIX camMat = RailCam->GetWorld();
	XMVECTOR camvec = { 0.0f,0.0f,0.0f,0.0f };

	camvec = XMVector3Transform(camvec, camMat);

	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.7f, 0.7f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.1f, 0.0f, 0.1f, 0.0f));
	ImGui::SetWindowSize(ImVec2(400, 500), ImGuiCond_::ImGuiCond_FirstUseEver);
	ImGui::Begin("Plyer");
	if (ImGui::TreeNode("Shake")) {
		ImGui::SliderFloat("ShakingValue", &ShakingValue, -100.0f, 100.0f);
		ImGui::TreePop();
	}


	if (ImGui::TreeNode("gunpos")) {
		ImGui::SliderFloat("gunpos.x", &GunPos.m128_f32[0], -100.0f, 100.0f);
		ImGui::SliderFloat("gunpos.y", &GunPos.m128_f32[1], -100.0f, 100.0f);
		ImGui::SliderFloat("gunpos.z", &GunPos.m128_f32[2], -100.0f, 100.0f);
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("gunWorldpos")) {
		ImGui::SliderFloat("gunWordpos.x", &GunNotParentPos.m128_f32[0], -100.0f, 100.0f);
		ImGui::SliderFloat("gunWordpos.y", &GunNotParentPos.m128_f32[1], -100.0f, 100.0f);
		ImGui::SliderFloat("gunWordpos.z", &GunNotParentPos.m128_f32[2], -100.0f, 100.0f);
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("ParticlePos")) {
		ImGui::SliderFloat("pos.x", &pos.x, -100.0f, 100.0f);
		ImGui::SliderFloat("pos.y", &pos.y, -100.0f, 100.0f);
		ImGui::SliderFloat("pos.z", &pos.z, -100.0f, 100.0f);
		ImGui::TreePop();
	}

	ImGui::End();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
}

void Player::SoundEffect()
{
	ShotSe->LoadFile("Resources/Sound/SE/shot.wav", 0.3f);
}

//�}�E�X����
void Player::MouseContoroll()
{
	//�}�E�X���W����p�x�̎擾
	GunRot.y = (ReticlePos2D.x - WinApp::window_width / 2) / 10;
	//�}�E�X���W�̎擾
	Mouse::GetInstance()->MouseMoveSprite(ReticlePos2D);
	//�e����������
	if (RecoilGunFlag == true) {
		ReticlePos2D.y += 15;
		Mouse::GetInstance()->RecoilMouse(ReticlePos2D);
	}
	else {
		//�擾�������W�����e�B�N���ɃZ�b�g
		SpriteReticle->SetPosition(ReticlePos2D);
	}
}

//��ʗh��
void Player::ScreenShake(float shakevalue, float shakingtime)
{
	if (ShakingStartFlag == true) {
		if (ShakeLimitTime <= 1) {
			ShakeLimitTime += shakingtime;
			if (ShakingScreenFlag == true) {
				ShakingScreenValue -= shakevalue;
				if (ShakingScreenValue <= -shakevalue) {
					ShakingScreenFlag = false;
				}
			}
			else {
				ShakingScreenValue += shakevalue;
				if (ShakingScreenValue >= shakevalue) {
					ShakingScreenFlag = true;
				}
			}
			EyeRot.x += ShakingScreenValue;
		}
		else {
			ShakingScreenFlag = true;
			ShakeLimitTime = 0;
			ShakingStartFlag = false;
			ShakingScreenValue = 0;
			EyeRot.x = 0;
		}
	}
}

//�_���[�W���󂯂��Ƃ��̏���
void Player::DamageProcess()
{
	if (OldHp > Hp) {
		ShakingStartFlag = true;
		ShakingValue = 3.5f;
		OldHp = Hp;
	}
}

//�e�̔��ˏ���
void Player::GunShotProcess(Phase paterncount)
{
	//�e�̔��ˑO
	if (playerstate_ == WAIT && Remaining < RemainingBullet) {
		if (Mouse::GetInstance()->PushClick(0)) {
			playerstate_ = SHOT;
			Remaining += 1;
			RecoilGunFlag = true;
		}
	}
	if (playerstate_ == SHOT) {
		//�e�����˂��ꂽ
		BulletShotFlag = true;
		ParticleEfect(paterncount);
		playerstate_ = WAIT;
	}
	else {
		BulletShotFlag = false;
	}
	RecoilProcess();
}

void Player::UIMotionProcess()
{
	if (Remaining >= 8) {
		if (ReversFlag == false) {
			Action::GetInstance()->EaseOut(ReloadSpriteSize.x, 260);
			Action::GetInstance()->EaseOut(ReloadSpriteSize.y, 190);
			if (ReloadSpriteSize.x >= 250) {
				ReversFlag = true;
			}
		}
		else {
			Action::GetInstance()->EaseOut(ReloadSpriteSize.x, 200);
			Action::GetInstance()->EaseOut(ReloadSpriteSize.y, 130);
			if (ReloadSpriteSize.x <= 210) {
				ReversFlag = false;
			}
		}
	}
	else if (Remaining == 0) {
		for (int i = 0; i < 9; i++) {
			SpritePos[i] = { 1220.0f,25.0f + 32.0f * i };
			SpriteRot[i] = 0;
			Time[i] = 0;
			DropBulletFlag[i] = false;
			OldRemaining = Remaining;
		}
	}
}

void Player::RecoilProcess()
{
	if (RecoilGunFlag == true) {
		RecoveryTime += 0.2f;
		GunRot.x = -25;
		GunPos.m128_f32[2] = -3.1f;
		if (RecoveryTime >= 1) {
			GunRot.x = 0;
			GunPos.m128_f32[2] = -3.0f;
			RecoveryTime = 0.0f;
			RecoilGunFlag = false;
		}
	}

}

//�����[�h����
void Player::ReloadProcess()
{

	if (playerstate_ == WAIT && Remaining != 0) {
		if (Mouse::GetInstance()->PushClick(1)) {
			playerstate_ = RELOAD;
			ReloadSe->LoadFile("Resources/Sound/SE/reload.wav", 0.3f);
			MouseStopFlag = true;
		}
	}

	if (playerstate_ == RELOAD) {
		GunRot.x -= 9.5f;
		Remaining = 8;
		ReloadTime += 1;
		Anser = ReloadTime % 40;
		if (Anser == 0) {
			Remaining = 0;
			GunRot.x = 0;
			if (Remaining == 0) {
				playerstate_ = WAIT;
				ReloadTime = 0;
				MouseStopFlag = false;
			}
		}
	}
}


//�}�Y���G�t�F�N�g
void Player::ParticleEfect(Phase paterncount)
{
	for (int i = 0; i < 10; i++) {
		float radX = ReticleRot.y * XM_PI / 180.f;
		float radY = GunRot.x * XM_PI / 180.f;
		float sinradX = sinf(radX);
		float cosradX = cosf(radX);

		float sinradY = sinf(radY);
		float cosradY = cosf(radY);

		if (paterncount == MOVEDPOINT_C || paterncount == MOVEDPOINT_C_OBLIQUE) {
			pos.x = GunWorldPos.m128_f32[0] + 2.3f;
			pos.y = GunWorldPos.m128_f32[1] - sinradY * 1.5f;
			pos.z = GunWorldPos.m128_f32[2] + 2.8f * cosradX;
		}
		else if (paterncount == LANDINGPOINT_BACK ||
			paterncount == MOVEDPOINT_A ||
			paterncount == MOVEDPOINT_A_LEFT ||
			paterncount == GOALPOINT_BACK) {
			pos.x = GunWorldPos.m128_f32[0] + sinradX * 3.5f;
			pos.y = GunWorldPos.m128_f32[1] - sinradY * 1.5f;
			pos.z = GunWorldPos.m128_f32[2] + 3.0f;
		}
		else if (paterncount == LANDINGPOINT_FRONT || paterncount == GOALPOINT) {
			pos.x = GunWorldPos.m128_f32[0] - sinradX * 3.5f;
			pos.y = GunWorldPos.m128_f32[1] - sinradY * 1.5f;
			pos.z = GunWorldPos.m128_f32[2] - 3.0f;
		}
		else if (paterncount == MOVEDPOINT_B) {
			pos.x = GunWorldPos.m128_f32[0] - 2.3f;
			pos.y = GunWorldPos.m128_f32[1] - sinradY * 1.5f;
			pos.z = GunWorldPos.m128_f32[2] + 2.8f * cosradX;
		}
		else if (paterncount == MOVEDPOINT_C_FRONT) {
			pos.x = GunWorldPos.m128_f32[0] + 2.3f * sinradX;
			pos.y = GunWorldPos.m128_f32[1] - sinradY * 1.5f;
			pos.z = GunWorldPos.m128_f32[2] + 2.8f * cosradX;
		}

		const float rnd_vel = 0.001f;
		XMFLOAT3 vel{};
		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

		const float smokernd_vel = 0.05f;
		XMFLOAT3 smokevel{};
		smokevel.x = (float)rand() / RAND_MAX * smokernd_vel - smokernd_vel / 2.0f;
		smokevel.y = (float)rand() / RAND_MAX * smokernd_vel - smokernd_vel / 2.0f;
		smokevel.z = (float)rand() / RAND_MAX * smokernd_vel - smokernd_vel / 2.0f;
		XMFLOAT3 acc{};
		acc.y = 0.0;

		XMFLOAT3 Smokeacc{};
		Smokeacc.y += 0.005f;
		PartRed->Add(20, pos, vel, acc, 0.7f, 0.2f, 1.0f);
		PartGreen->Add(20, pos, vel, acc, 0.5f, 0.2f, 1.0f);
		PartSmoke->Add(50, pos, smokevel, acc, 0.5f, 0.0f, 1.0f);
	}
	SoundEffect();

}


