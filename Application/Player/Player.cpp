#include "Player.h"
#include<cassert>
#include "imgui.h"
#include"imconfig.h"
#include"ModelManager.h"
#include"Mouse.h"
#include"WinApp.h"

const int ReaminingBullet = 8;

//デストラクタ
Player::~Player()
{
	delete PartGreen;
	delete ShotSe;
	delete ReloadSe;

	Gun.reset();
	Body.reset();
}

//初期化処理
void Player::Initalize(Camera* camera)
{

	Object3d::SetCamera(camera);

	SkipPos = CurtainDownPos;
	Sprite::LoadTexture(200, L"Resources/mark2.png");
	Sprite::LoadTexture(300, L"Resources/curtain.png");
	Sprite::LoadTexture(302, L"Resources/Skip.png");

	SpriteReticle.reset(Sprite::SpriteCreate(200, ReticlePos2D, ReticleColor, ReticleAncorPoint));
	CurtainUp.reset(Sprite::SpriteCreate(300, CurtainUpPos));
	CurtainDown.reset(Sprite::SpriteCreate(300, CurtainDownPos));
	Skip.reset(Sprite::SpriteCreate(302, SkipPos));

	Gun = Object3d::Create(ModelManager::GetInstance()->GetModel(10));
	Body = Object3d::Create(ModelManager::GetInstance()->GetModel(10));

	CurtainUp->SetSize(CurtainSize);
	CurtainDown->SetSize(CurtainSize);

	RailCam = new RailCamera();
	RailCam->MatrixIdentity(ReticlePos, ReticleRot);

	Body->SetParent(camera);

	PartGreen = ParticleManager::Create(camera);
	PartRed = ParticleManager::Create(camera);
	EyeRot.y = 180;
	RailCam->Update(vel, EyeRot, camera);

	ShotSe = new Audio();
	ShotSe->Initialize();

	ReloadSe = new Audio();
	ReloadSe->Initialize();
};

//ステータスセット
void Player::StatusSet(Camera* camera)
{
	BodyMat = Body->GetMatrix();
	BodyWorldPos = { -10.0f,0.0f,-20.0f };
	BodyWorldPos = XMVector3Transform(BodyWorldPos, BodyMat);

	ReticleRot.y = (ReticlePos2D.x - 640) / 10 + ChangeRot;

	Body->SetRotation(BodyRot);
	Body->SetPosition(BodyPos);
	Body->SetScale(BodyScl);
	Body->SetParent(camera);

	Gun->SetRotation(GunRot);
	Gun->SetScale(GunScl);
	Gun->SetParent(camera);
	Gun->SetPosition(GunPos);

	GunMat = Gun->GetMatrix();
	GunWorldPos = XMVector3Transform(GunPos, GunMat);
}

//更新処理
void Player::Update(int& Remaining, Camera* camera, int paterncount)
{
	PaternCount = paterncount;

	//マウス座標の取得
	GunRot.y = (ReticlePos2D.x - WinApp::window_width / 2) / 10;
	MouthContoroll();
	if (MouseStop_F == false) {
		GunRot.x = (ReticlePos2D.y - WinApp::window_height / 2) / 50;
	}
	else {
		GunRot.x += 10.0f;
	}

	if (CameraWork_F == true) {
		//弾の発射前
		if (Mouse::GetInstance()->PushClick(0)) {
			if (Remaining < ReaminingBullet && ReloadFlag == false && BulletShot_F == false) {
				Remaining += 1;
				Particle_F = true;
				BulletShot_F = true;
				Recoil_F = true;
				ShakingStart = true;
				RecoilGun = true;
			}
		}
		else {
			BulletShot_F = false;
		}


		if (RecoilGun == true) {
			RecoveryTime += 0.2f;
			GunPos.m128_f32[2] = -3.5f;
			if (RecoveryTime >= 1) {
				GunPos.m128_f32[2] = -3.0f;
				RecoveryTime = 0.0f;
				RecoilGun = false;
			}
		}

		if (ShakingStart == true) {
			ScreenShake(0.4f, 0.5f);
		}

		ParticleEfect();
		//リロード
		if ((Mouse::GetInstance()->PushClick(1)) && Remaining != 0) {
			if (ReloadSound_F == true) {
				ReloadSe->LoadFile("Resources/Sound/SE/reload.wav", 0.3f);
				ReloadSound_F = false;
				MouseStop_F = true;
			}
			ReloadFlag = true;
		}

		if (ReloadFlag == true) {
			Remaining = 8;
			ReloadTime += 1;
			Anser = ReloadTime % 40;
			if (Anser == 0) {
				Remaining = 0;
				GunRot.x = 0;
				if (Remaining == 0) {
					ReloadFlag = false;
					ReloadSound_F = true;
					ReloadTime = 0;
					MouseStop_F = false;
				}
			}
		}
	}
	if (Move_F == false) {
		MoveSpeed = 0.0f;
		vel = { 0, 0, MoveSpeed };
	}

	vel = XMVector3TransformNormal(vel, BodyMat);

	CameraWork();

	StatusSet(camera);

	RailCam->Update(vel, EyeRot, camera);

	Body->Update();
	Gun->Update();
	PartRed->Update({ 1.0f,0.0f,0.0f,0.0f });
	PartGreen->Update({ 0.0f,0.5f,0,0.0f });
}

//パーティクル描画
void Player::ParticleDraw(ID3D12GraphicsCommandList* cmdeList)
{
	ParticleManager::PreDraw(cmdeList);
	PartRed->Draw();
	PartGreen->Draw();
	ParticleManager::PostDraw();
}

//スプライト描画
void Player::SpriteDraw()
{
	if (CameraWork_F == true && MouseStop_F == false) {
		SpriteReticle->Draw();
	}
	else {
		CurtainUp->Draw();
		CurtainDown->Draw();
		Skip->Draw();
	}
}

//カメラワーク
void Player::CameraWork()
{
	if (CameraWork_F == false && Start_F == false) {

		if (stanby == false) {
			EyeRot.y = 180;
		}
		else if (stanby == true && ActionCount == 0) {
			Action::GetInstance()->EaseOut(EyeRot.y, -5.0f);
			//後ろを向く
			if (EyeRot.y <= 0) {
				EyeRot.y = 0;
				ActionTimer += 0.2f;
				if (ActionTimer > 5) {
					ActionTimer = 5.0f;
					Action::GetInstance()->EaseOut(EyeRot.x, 95.0f);
				}
			}
			//下を向く
			if (EyeRot.x >= 90) {
				ActionTimer = 0.0f;
				EyeRot.x = 90;
				ActionCount = 1;
			}
		}
		if (ActionCount == 1) {

			ActionTimer += 0.15f;
			vel = { 0.0f,0.67f,0.4f };
			if (ActionTimer >= 5) {
				vel = { 0.0f,-0.6f,0.0f };
				Action::GetInstance()->EaseOut(EyeRot.x, -5.0f);
			}
			if (EyeRot.x <= 0.0f) {
				EyeRot.x = 0.0f;
			}
			if (BodyWorldPos.m128_f32[1] <= 0.3f) {
				vel = { 0.0f,0.0f,0.0f };
				ReticlePos.m128_f32[1] = 0.0f;
				MovieFlag = true;
			}
		}

	}

	if ((Mouse::GetInstance()->PushClick(1) || Mouse::GetInstance()->PushClick(0)) && stanby == true && CameraWork_F == false) {
		MovieFlag = true;
		ActionCount = 100;
		EyeRot.x = 0;
		EyeRot.y = 0;
		vel = { 0.0f,0.0f,0.0f };
		ReticlePos = { 0.0f,-0.7f,13.0f };
		RailCam->MatrixIdentity(ReticlePos, EyeRot);
	}

	if (stanby == false) {
		ActionTimer += 0.01f;
		if (ActionTimer >= 1.0f) {
			ActionTimer = 0.0f;
			stanby = true;
		}
	}

	if (MovieFlag == false) {
		CurtainUpPos.y += 4;
		CurtainDownPos.y -= 4;
		SkipPos.y -= 2;

		if (CurtainUpPos.y >= 0) {
			CurtainUpPos.y = 0;
		}

		if (CurtainDownPos.y <= 620) {
			CurtainDownPos.y = 620;
		}

		if (SkipPos.y <= 620) {
			SkipPos.y = 620;
		}
	}
	else {
		CurtainUpPos.y -= 4;
		CurtainDownPos.y += 4;
		SkipPos.y += 4;

		if (CurtainUpPos.y <= -100) {
			CurtainUpPos.y = -100;
		}

		if (CurtainDownPos.y >= 720) {
			CurtainDownPos.y = 720;
			CameraWork_F = true;
			Start_F = true;
		}

		if (SkipPos.y >= 720) {
			SkipPos.y = 12000;
		}
	}
	CurtainUp->SetPosition(CurtainUpPos);
	CurtainDown->SetPosition(CurtainDownPos);
	Skip->SetPosition(SkipPos);
}

//プレイヤー移動
void Player::PlayerMove(bool& move, int patern)
{
	XMMATRIX camMat = RailCam->GetWorld();
	XMVECTOR camvec = { 0.0f,0.0f,0.0f,0.0f };

	camvec = XMVector3Transform(camvec, camMat);

	PaternCount = patern;
	//敵をすべて倒した時
	if (move == true) {
		Move_F = true;
	}

	if (Move_F == true) {
		MoveSpeed = 0.5f;
		if (ShakeHead_F == true) {
			if (shake == 0) {
				EyeRot.x += 0.05f;
				if (EyeRot.x >= 0.5f) {
					shake = 1;
				}
			}
			if (shake == 1) {
				EyeRot.x -= 0.05f;
				if (EyeRot.x <= -0.5f) {
					shake = 0;
				}
			}
		}
		if (patern == 0) {
			vel = { 0, 0, MoveSpeed };
			EnemyCount = 3;
			if (camvec.m128_f32[2] >= 20) {
				move = false;
				Move_F = false;
				WaveCount += 1;
				MoveTimer = 0.0f;
				Finish = true;
				EyeRot.x = 0;
			}
			else {
				Distance = 11;
			}
		}
		else if (patern == 1) {
			EnemyCount = 4;
			vel = { 0, 0, MoveSpeed };
			if (camvec.m128_f32[2] >= 40) {
				move = false;
				Move_F = false;
				WaveCount += 1;
				MoveTimer = 0.0f;
				Finish = true;
			}
			else {
				Distance = 11;
			}
		}
		else if (patern == 2) {
			EnemyCount = 4;
			Action::GetInstance()->EaseOut(EyeRot.y, 95.0f);
			if (EyeRot.y >= 90) {
				EyeRot.y = 90;
				ChangeRot = -90;
				vel = { 0, 0, MoveSpeed };
			}
			if (camvec.m128_f32[0] >= 30) {
				move = false;
				Move_F = false;
				WaveCount += 1;
				MoveTimer = 0.0f;
				Finish = true;
			}
			else {
				Distance = 11;
			}
		}
		else if (patern == 3) {
			EnemyCount = 3;
			vel = { 0, 0, MoveSpeed };

			if (camvec.m128_f32[0] >= 45) {
				move = false;
				Move_F = false;
				WaveCount += 1;
				MoveTimer = 0.0f;
				Finish = true;
			}
			else {
				Distance = 11;
			}
		}
		else if (patern == 4) {
			EnemyCount = 5;
			Action::GetInstance()->EaseOut(EyeRot.y, -5.0f);
			if (EyeRot.y <= 0) {
				ChangeRot = 0;
				EyeRot.y = 0;
				vel = { 0, 0, MoveSpeed };
			}
			if (camvec.m128_f32[2] >= 70) {
				move = false;
				Move_F = false;
				WaveCount += 1;
				MoveTimer = 0.0f;
				Finish = true;
			}
			else {
				Distance = 11;
			}
		}
		else if (patern == 5) {
			EyeRot.y -= 3;
			if (EyeRot.y <= 0) {
				EyeRot.y = 0;
				vel = { 0, 0, MoveSpeed };
			}
			if (camvec.m128_f32[2] >= 90) {
				move = false;
				Move_F = false;
				WaveCount += 1;
				MoveTimer = 0.0f;
				Finish = true;
			}
			else {
				Distance = 11;
			}
		}
		else if (patern == 6) {
			stanby = false;
			vel = { 0, 0, 0.1 };
			ShakeHead_F = false;
			if (camvec.m128_f32[2] >= 92) {
				vel = { 0,0.05,0.1 };
				if (camvec.m128_f32[2] >= 97) {
					vel = { 0.0f,0.0f,0.0f };
					Fring_F = true;
					if (Fring_F == true) {
						vel = { 0.0f,0.665f,0.0f };
					}
				}
			}
			move = false;
			CameraWork_F = false;

			WaveCount += 1;
			MoveTimer = 0.0f;
			MovieFlag = false;
			ActionCount = 0;
		}
	}
}

//オブジェクト描画
void Player::ObjDraw()
{
	if (Hp >= 0 && CameraWork_F == true) {
		Gun->Draw();
	}
}

void Player::ImGuiDraw()
{

	XMMATRIX camMat = RailCam->GetWorld();
	XMVECTOR camvec = { 0.0f,0.0f,0.0f,0.0f };

	camvec = XMVector3Transform(camvec, camMat);

	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.7f, 0.7f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.1f, 0.0f, 0.1f, 0.0f));
	ImGui::SetWindowSize(ImVec2(400, 500), ImGuiCond_::ImGuiCond_FirstUseEver);
	ImGui::Begin("Plyer");
	if (ImGui::TreeNode("LoacalPos")) {
		ImGui::SliderFloat("pos.x", &pos.x, -100.0f, 100.0f);
		ImGui::SliderFloat("pos.y", &pos.y, -100.0f, 100.0f);
		ImGui::SliderFloat("pos.z", &pos.z, -100.0f, 100.0f);
		ImGui::TreePop();
	}


	if (ImGui::TreeNode("ReticleRot")) {
		ImGui::SliderFloat("Rot.x", &ReticleRot.x, -100.0f, 100.0f);
		ImGui::SliderFloat("Rot.y", &ReticleRot.y, -100.0f, 100.0f);
		ImGui::SliderFloat("Rot.z", &ReticleRot.z, -100.0f, 100.0f);
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("ReticlePos2D")) {
		ImGui::SliderFloat("ReticlePos2D.x", &ReticlePos2D.x, 0.0f, 1280.0f);
		ImGui::SliderFloat("ReticlePos2D.y", &ReticlePos2D.y, 0.0f, 720.0f);
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("gunpos")) {
		ImGui::SliderFloat("gunpos.x", &GunPos.m128_f32[0], -100.0f, 100.0f);
		ImGui::SliderFloat("gunpos.y", &GunPos.m128_f32[1], -100.0f, 100.0f);
		ImGui::SliderFloat("gunpos.z", &GunPos.m128_f32[2], -100.0f, 100.0f);
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("gunWorldpos")) {
		ImGui::SliderFloat("gunWordpos.x", &GunWorldPos.m128_f32[0], -100.0f, 100.0f);
		ImGui::SliderFloat("gunWordpos.y", &GunWorldPos.m128_f32[1], -100.0f, 100.0f);
		ImGui::SliderFloat("gunWordpos.z", &GunWorldPos.m128_f32[2], -100.0f, 100.0f);
		ImGui::TreePop();
	}

	ImGui::End();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
}

void Player::ChangeViewPort(XMMATRIX& matViewPort)
{
	matViewPort.r[0].m128_f32[0] = WinApp::window_width / 2;
	matViewPort.r[0].m128_f32[1] = 0;
	matViewPort.r[0].m128_f32[2] = 0;
	matViewPort.r[0].m128_f32[3] = 0;

	matViewPort.r[1].m128_f32[0] = 0;
	matViewPort.r[1].m128_f32[1] = -(WinApp::window_height / 2);
	matViewPort.r[1].m128_f32[2] = 0;
	matViewPort.r[1].m128_f32[3] = 0;

	matViewPort.r[2].m128_f32[0] = 0;
	matViewPort.r[2].m128_f32[1] = 0;
	matViewPort.r[2].m128_f32[2] = 1;
	matViewPort.r[2].m128_f32[3] = 0;

	matViewPort.r[3].m128_f32[0] = WinApp::window_width / 2 + offset.m128_f32[0];
	matViewPort.r[3].m128_f32[1] = WinApp::window_height / 2 + offset.m128_f32[1];
	matViewPort.r[3].m128_f32[2] = 0;
	matViewPort.r[3].m128_f32[3] = 1;
}

void Player::SoundEffect()
{
	ShotSe->LoadFile("Resources/Sound/SE/shot.wav", 0.3f);
}

//マウス操作
void Player::MouthContoroll()
{
	//マウス座標の取得
	Mouse::GetInstance()->MouseMoveSprite(ReticlePos2D);
	if (Recoil_F == true) {
		Mouse::GetInstance()->RecoilMouse(ReticlePos2D);
		Recoil_F = false;
	}
	else {
		//取得した座標をレティクルにセット
		SpriteReticle->SetPosition(ReticlePos2D);
	}
}

void Player::ScreenShake(float shakevalue, float shakingtime)
{
	if (ShakeLimitTime <= 1) {
		ShakeLimitTime += shakingtime;
		if (ShakingScreen_F == true) {
			ShakingScreenValue -= shakevalue;
			if (ShakingScreenValue <= -shakevalue) {
				ShakingScreen_F = false;
			}
		}
		else {
			ShakingScreenValue += shakevalue;
			if (ShakingScreenValue >= shakevalue) {
				ShakingScreen_F = true;
			}
		}
		EyeRot.x += ShakingScreenValue;
	}
	else {
		ShakeLimitTime = 0;
		ShakingStart = false;
		EyeRot.x = 0;
	}
}

//マズルエフェクト
void Player::ParticleEfect()
{
	if (Particle_F == true) {
		for (int i = 0; i < 10; i++) {
			double radX = ReticleRot.y * M_PI / 180;
			double radY = GunRot.x * M_PI / 180;
			float sinradX = sinf(radX);
			float cosradX = cosf(radX);

			float sinradY = sinf(radY);
			float cosradY = cosf(radY);
			if (PaternCount == 3 || PaternCount == 4) {
				pos.x = GunWorldPos.m128_f32[0] + 2.3f;
				pos.y = GunWorldPos.m128_f32[1] - sinradY * 3;
				pos.z = GunWorldPos.m128_f32[2] - 2.8f * cosradX;
			}
			else if (PaternCount == 0 || PaternCount == 1 || PaternCount == 2 || PaternCount == 5) {
				pos.x = GunWorldPos.m128_f32[0] + sinradX * 2.8f;
				pos.y = GunWorldPos.m128_f32[1] - sinradY * 3;
				pos.z = GunWorldPos.m128_f32[2] + 2.3f;
			}

			const float rnd_vel = 0.001f;
			XMFLOAT3 vel{};
			vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
			vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
			vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
			XMFLOAT3 acc{};
			acc.y = -0.00001;

			PartRed->Add(10, pos, vel, acc, 0.7f, 0.2f, 1.0f);
			PartGreen->Add(10, pos, vel, acc, 0.5f, 0.2f, 1.0f);
		}
		Particle_F = false;
		SoundEffect();
	}

}


