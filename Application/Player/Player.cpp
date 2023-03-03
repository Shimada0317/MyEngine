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
	delete PartRed;
	delete PartSmoke;
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
	Sprite::LoadTexture(303, L"Resources/bullet.png");
	Sprite::LoadTexture(304, L"Resources/Smoke.png");
	Sprite::LoadTexture(10, L"Resources/Reload.png");
	//スプライトの読み込み
	for (int i = 0; i < 9; i++) {
		SpritePos[i] = { 1220.0f,25.0f + 32.0f * i };
		SpriteRot[i] = 0;
		Time[i] = 0;
		bulletHUD[i].reset(Sprite::SpriteCreate(303, SpritePos[i], { 1.0f,1.0f,1.0f,1.0f }, AnchorPoint));
		Smoke[i].reset(Sprite::SpriteCreate(304, SpritePos[i], { 1.0f,1.0f,1.0f,1.0f }, AnchorPoint));
		DropBulletFlag[i] = false;
	}

	OldRemaining = Remaining;

	Reload.reset(Sprite::SpriteCreate(10, ReloadSpritePos, ReloadSpriteColor, AnchorPoint));

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
	PartSmoke = ParticleManager::Create(camera);
	EyeRot.y = 180;
	OldHp = Hp;
	RailCam->Update(Velocity, EyeRot, camera);

	ShotSe = new Audio();
	ShotSe->Initialize();

	ReloadSe = new Audio();
	ReloadSe->Initialize();
};

//ステータスセット
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


	//GunNotParentPos.m128_f32[2] = -1.0f;
	GunMat = Gun->GetMatrix();
	GunWorldPos = XMVector3Transform(GunPos, GunMat);

	Gun->SetRotation(GunRot);
	Gun->SetScale(GunScl);
	Gun->SetParent(camera);
	XMMATRIX GunNotParentMatrix = Gun->GetNotParentWorld();

	GunNotParentPos = XMVector3Transform(GunPos, GunNotParentMatrix);
	Gun->SetPosition(GunPos);

	//HUDのポジションセット
	for (int i = 0; i < 9; i++) {
		bulletHUD[i]->SetSize({ SpriteSiz });
		bulletHUD[i]->SetPosition(SpritePos[i]);
		bulletHUD[i]->SetRotation(SpriteRot[i]);
	}


	//リロードの文字
	Reload->SetSize(ReloadSpriteSize);
	Reload->SetColor(ReloadSpriteColor);
}


void Player::AllUpdate(Camera* camera)
{
	RailCam->Update(Velocity, EyeRot, camera);
	Body->Update();
	Gun->Update();
	PartRed->Update({ 1.0f,0.0f,0.0f,0.0f });
	PartGreen->Update({ 0.0f,0.5f,0,0.0f });
	PartSmoke->Update({ 0.1f,0.1f,0.1f,0.0f });
}


//更新処理
void Player::Update(Camera* camera, int paterncount)
{
	PaternCount = paterncount;

	DamageProcess();

	//マウス座標の取得
	GunRot.y = (ReticlePos2D.x - WinApp::window_width / 2) / 10;
	MouthContoroll();
	if (MouseStopFlag == false) {
		GunRot.x = (ReticlePos2D.y - WinApp::window_height / 2) / 50;
	}
	else {
		GunRot.x += 10.0f;
	}

	//カメラが動いていないとき
	if (CameraWorkFlag == true) {

		GunShotProcess();

		ScreenShake(ShakingValue, 0.1f);

		ParticleEfect();

		ReloadProcess();
	}

	Velocity = XMVector3TransformNormal(Velocity, BodyMat);

	CameraWork();

	StatusSet(camera);

	AllUpdate(camera);
}

//パーティクル描画
void Player::ParticleDraw(ID3D12GraphicsCommandList* cmdeList)
{
	ParticleManager::PreDraw(cmdeList);
	PartSmoke->Draw();
	PartRed->Draw();
	PartGreen->Draw();
	ParticleManager::PostDraw();
}

//スプライト描画
void Player::SpriteDraw()
{
	if (CameraWorkFlag == true && MouseStopFlag == false) {
		SpriteReticle->Draw();
		for (int i = 0; i < 8; i++) {
			if (Remaining <= 8 && ReloadFlag == false) {
				//Smoke[i]->Draw();
				bulletHUD[i]->Draw();
			}
		}

		if (Remaining >= 8) {
			Reload->Draw();
		}
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
	if (CameraWorkFlag == false && StartFlag == false) {

		if (StanbyFlag == false) {
			EyeRot.y = 180;
		}
		else if (StanbyFlag == true && ActionCount == 0) {
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
			Velocity = { 0.0f,0.67f,0.4f };
			if (ActionTimer >= 5) {
				Velocity = { 0.0f,-0.6f,0.0f };
				Action::GetInstance()->EaseOut(EyeRot.x, -5.0f);
			}
			if (EyeRot.x <= 0.0f) {
				EyeRot.x = 0.0f;
			}
			if (BodyWorldPos.m128_f32[1] <= 0.3f) {
				Velocity = { 0.0f,0.0f,0.0f };
				ReticlePos.m128_f32[1] = 0.0f;
				MovieFlag = true;
			}
		}

	}

	if ((Mouse::GetInstance()->PushClick(1) || Mouse::GetInstance()->PushClick(0)) && StanbyFlag == true && CameraWorkFlag == false) {
		MovieFlag = true;
		ActionCount = 100;
		EyeRot.x = 0;
		EyeRot.y = 0;
		Velocity = { 0.0f,0.0f,0.0f };
		ReticlePos = { 0.0f,-0.7f,13.0f };
		RailCam->MatrixIdentity(ReticlePos, EyeRot);
	}

	if (StanbyFlag == false) {
		ActionTimer += 0.01f;
		if (ActionTimer >= 1.0f) {
			ActionTimer = 0.0f;
			StanbyFlag = true;
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
			CameraWorkFlag = true;
			StartFlag = true;
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
		MoveFlag = true;
	}

	if (MoveFlag == true) {
		MoveSpeed = 0.5f;
		if (ShakeHeadFlag == true) {
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
			Velocity = { 0, 0, MoveSpeed };
			EnemyCount = 3;
			if (camvec.m128_f32[2] >= 20) {
				move = false;
				MoveFlag = false;
				WaveCount += 1;
				MoveTimer = 0.0f;
				FinishFlag = true;
				EyeRot.x = 0;
			}
			else {
				Distance = 11;
			}
		}
		else if (patern == 1) {
			EnemyCount = 4;
			Velocity = { 0, 0, MoveSpeed };
			if (camvec.m128_f32[2] >= 40) {
				move = false;
				MoveFlag = false;
				WaveCount += 1;
				MoveTimer = 0.0f;
				FinishFlag = true;
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
				Velocity = { 0, 0, MoveSpeed };
			}
			if (camvec.m128_f32[0] >= 30) {
				move = false;
				MoveFlag = false;
				WaveCount += 1;
				MoveTimer = 0.0f;
				FinishFlag = true;
			}
			else {
				Distance = 11;
			}
		}
		else if (patern == 3) {
			EnemyCount = 3;
			Velocity = { 0, 0, MoveSpeed };

			if (camvec.m128_f32[0] >= 45) {
				move = false;
				MoveFlag = false;
				WaveCount += 1;
				MoveTimer = 0.0f;
				FinishFlag = true;
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
				Velocity = { 0, 0, MoveSpeed };
			}
			if (camvec.m128_f32[2] >= 70) {
				move = false;
				MoveFlag = false;
				WaveCount += 1;
				MoveTimer = 0.0f;
				FinishFlag = true;
			}
			else {
				Distance = 11;
			}
		}
		else if (patern == 5) {
			EyeRot.y -= 3;
			if (EyeRot.y <= 0) {
				EyeRot.y = 0;
				Velocity = { 0, 0, MoveSpeed };
			}
			if (camvec.m128_f32[2] >= 90) {
				move = false;
				MoveFlag = false;
				WaveCount += 1;
				MoveTimer = 0.0f;
				FinishFlag = true;
			}
			else {
				Distance = 11;
			}
		}
		else if (patern == 6) {
			StanbyFlag = false;
			Velocity = { 0, 0, 0.1 };
			ShakeHeadFlag = false;
			if (camvec.m128_f32[2] >= 92) {
				Velocity = { 0,0.05,0.1 };
				if (camvec.m128_f32[2] >= 97) {
					Velocity = { 0.0f,0.0f,0.0f };
					FringFlag = true;
					if (FringFlag == true) {
						Velocity = { 0.0f,0.6683f,0.0f };
					}
				}
			}
			move = false;
			CameraWorkFlag = false;

			WaveCount += 1;
			MoveTimer = 0.0f;
			MovieFlag = false;
			ActionCount = 0;
		}
	}
	else if (MoveFlag == false) {
		Velocity = { 0.f,0.f,0.f };
	}
}

//オブジェクト描画
void Player::ObjDraw()
{
	if (Hp >= 0 && CameraWorkFlag == true) {
		Gun->Draw();
	}
}

//ImgUi描画
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
	if (RecoilFlag == true) {
		Mouse::GetInstance()->RecoilMouse(ReticlePos2D);
		RecoilFlag = false;
	}
	else {
		//取得した座標をレティクルにセット
		SpriteReticle->SetPosition(ReticlePos2D);
	}
}

//画面揺れ
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

//ダメージを受けたときの処理
void Player::DamageProcess()
{
	if (OldHp > Hp) {
		ShakingStartFlag = true;
		ShakingValue = 3.5f;
		OldHp = Hp;
	}
}

//弾の発射処理
void Player::GunShotProcess()
{
	//弾の発射前
	if (Mouse::GetInstance()->PushClick(0)) {
		if (Remaining < ReaminingBullet && ReloadFlag == false && BulletShotFlag == false) {
			Remaining += 1;
			//パーティクル発生フラグ
			ParticleFlag = true;
			//弾の発射フラグ
			BulletShotFlag = true;
			//2Dスプライトリコイルフラグ
			RecoilFlag = true;
			//画面揺れのフラグ
			ShakingStartFlag = true;
			//銃オブジェクトリコイルフラグ
			RecoilGunFlag = true;
			ShakingValue = 0.6f;
		}
	}
	else {
		BulletShotFlag = false;
	}

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

//リロード処理
void Player::ReloadProcess()
{
	//右クリックを押した時
	if ((Mouse::GetInstance()->PushClick(1)) && Remaining != 0) {
		if (ReloadSoundFlag == true) {
			ReloadSe->LoadFile("Resources/Sound/SE/reload.wav", 0.3f);
			ReloadSoundFlag = false;
			MouseStopFlag = true;
		}
		ReloadFlag = true;
	}

	if (Remaining >= 8) {
		if (ReversFlag == false) {
			Action::GetInstance()->EaseOut(ReloadSpriteSize.x, 210);
			Action::GetInstance()->EaseOut(ReloadSpriteSize.y, 140);
			if (ReloadSpriteSize.x >= 200) {
				ReversFlag = true;
			}
		}
		else {
			Action::GetInstance()->EaseOut(ReloadSpriteSize.x, 150);
			Action::GetInstance()->EaseOut(ReloadSpriteSize.y, 80);
			if (ReloadSpriteSize.x <= 160) {
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

	if (ReloadFlag == true) {
		Remaining = 8;
		ReloadTime += 1;
		Anser = ReloadTime % 40;
		if (Anser == 0) {
			Remaining = 0;
			GunRot.x = 0;
			if (Remaining == 0) {
				ReloadFlag = false;
				ReloadSoundFlag = true;
				ReloadTime = 0;
				MouseStopFlag = false;
			}
		}
	}
}

//マズルエフェクト
void Player::ParticleEfect()
{
	if (ParticleFlag == true) {
		for (int i = 0; i < 10; i++) {
			double radX = ReticleRot.y * M_PI / 180;
			double radY = GunRot.x * M_PI / 180;
			float sinradX = sinf(radX);
			float cosradX = cosf(radX);

			float sinradY = sinf(radY);
			float cosradY = cosf(radY);

			XMFLOAT3 pos;
			
			if (PaternCount == 3 || PaternCount == 4) {
				pos.x = GunWorldPos.m128_f32[0] + 2.3f;
				pos.y = GunWorldPos.m128_f32[1] - sinradY * 1.5f;
				pos.z = GunWorldPos.m128_f32[2] - 2.8f * cosradX;
			}
			else if (PaternCount == 0 || PaternCount == 1 || PaternCount == 2 || PaternCount == 5) {
				pos.x = GunWorldPos.m128_f32[0] + sinradX * 3.5f;
				pos.y = GunWorldPos.m128_f32[1] - sinradY * 1.5f;
				pos.z = GunWorldPos.m128_f32[2] + 3.0f;
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
		ParticleFlag = false;
		SoundEffect();
	}

}


