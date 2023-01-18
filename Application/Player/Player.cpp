#include "Player.h"
#include<cassert>
#include "imgui/imgui.h"
#include"imgui/imconfig.h"
#include"WinApp.h"

Player::~Player()
{
	delete TrackModel;
	delete GunModel;
	delete PlayerModel;
	delete PartGreen;
	delete Shot_SE;
	delete Reload_SE;

	Track.reset();
	Gun.reset();
	Player.reset();
}

void Player::Initalize(Camera* came)
{

	Object3d::SetCamera(came);

	TrackModel = ObjModel::CreateFromOBJ("tst2");
	Track = Object3d::Create(TrackModel);

	GunModel = ObjModel::CreateFromOBJ("gun");
	Gun = Object3d::Create(GunModel);

	PlayerModel = ObjModel::CreateFromOBJ("gun");
	Player = Object3d::Create(PlayerModel);

	Sprite::LoadTexture(200, L"Resources/mark2.png");
	SpriteRet.reset(Sprite::SpriteCreate(200, retpos, spCol, anc));

	Sprite::LoadTexture(300, L"Resources/curtain.png");
	Curtain_Up.reset(Sprite::SpriteCreate(300, curtainPos));

	Sprite::LoadTexture(301, L"Resources/Window.png");
	Window.reset(Sprite::SpriteCreate(301, windowPos));

	skipPos = curtainPos2;

	Sprite::LoadTexture(302, L"Resources/Skip.png");
	Skip.reset(Sprite::SpriteCreate(302, skipPos));

	Curtain_Down.reset(Sprite::SpriteCreate(300, curtainPos2));
	Curtain_Up->SetSize(curtainSiz);
	Curtain_Down->SetSize(curtainSiz);

	Railcam = new RailCamera();
	Railcam->Initialize(Ret_Pos, Ret_Rot);

	Player->SetParent(came);

	PartGreen = ParticleManager::Create(came);
	PartRed = ParticleManager::Create(came);
	Eye_rot.y = 180;
	Railcam->Updata(vel, Eye_rot, came);

	Shot_SE = new Audio();
	Shot_SE->Initialize();

	Reload_SE = new Audio();
	Reload_SE->Initialize();
};

void Player::Set(Camera* came)
{
	const float BulletSpeed = 15.0f;
	velocity = { 0, 0, BulletSpeed };
	velocity = XMVector3TransformNormal(velocity, Track_Mat);
	TrackWorld_Pos = { 0.0f,0.0f,2.0f };
	TrackWorld_Pos = XMVector3Transform(TrackWorld_Pos, Track_Mat);

	Player_Mat = Player->GetMatrix();
	PlayerWorld_Pos = { -10.0f,0.0f,-20.0f };
	PlayerWorld_Pos = XMVector3Transform(PlayerWorld_Pos, Player_Mat);

	Ret_Rot.y = (retpos.x - 640) / 10 + ChangeRot;
	//レティクルObj
	Track->SetRotation({ Ret_Rot });
	Track->SetScale({ Ret_Scl });
	Track_Mat = Track->GetMatrix();

	//プレイヤーObj
	Player->SetRotation(Player_Rot);
	Player->SetPosition(Player_Pos);
	Player->SetScale(Player_Scl);
	Player->SetParent(came);

	Gun_Rot.y = (retpos.x - 640) / 10;
	Gun_Rot.x = (retpos.y - 360) / 50;


	Gun->SetRotation(Gun_Rot);
	Gun->SetScale(Gun_Scl);
	Gun->SetParent(came);
	Gun->SetPosition(Gun_Pos);

	Gun_Mat = Gun->GetMatrix();
	GunWorld_Pos = { 0.0f,0.0f,1.0f };
	GunWorld_Pos = XMVector3Transform(Gun_Pos, Gun_Mat);
}

void Player::Updata(Bullet* bull[], int& Remaining, const XMVECTOR enePos[], Camera* came, const XMFLOAT2 Ene2dPos[], int pat)
{
	PaternCount = pat;
	if (CamWork == true) {
		//弾の発射前
		if (Remaining < BULL - 1 && ReloadFlag == false) {
			if (Mouse::GetInstance()->PushClick(0)) {
				Remaining += 1;
				particle = true;
				for (int i = 0; i < BULL; i++) {
					if (bull[i]->CheckOk()) {
						bull[i]->Test(GunWorld_Pos, TrackWorld_Pos, Eye_rot,Ret_Rot);
						bull[i]->TriggerOn();
						break;
					}
				}
			}
		}
		ParticleEfect();
		//リロード
		if ((Mouse::GetInstance()->PushClick(1)) && Remaining != 0) {
			if (ReloadSound_F == true) {
				Reload_SE->LoadFile("Resources/Sound/SE/reload.wav", 0.3f);
				ReloadSound_F = false;
			}
			ReloadFlag = true;
		}

		if (ReloadFlag == true) {
			Remaining = 8;
			ReloadTime += 1;
			Anser = ReloadTime % 40;
			if (Anser == 0) {
				Remaining = 0;
				if (Remaining == 0) {
					ReloadFlag = false;
					ReloadSound_F = true;
					ReloadTime = 0;
				}
			}
		}
	}
	if (Active == false) {
		kBulletSpeed = 0.0f;
		vel = { 0, 0, kBulletSpeed };
		//Eye_rot.x = 0;
	}

	vel = XMVector3TransformNormal(vel, Player_Mat);

	for (int i = 0; i < 9; i++) {
		bull[i]->Updata();
	}

	MouthContoroll(enePos, came, Ene2dPos);

	CameraWork();

	Set(came);
	Railcam->Updata(vel, Eye_rot, came);

	Track->Updata();
	Player->Updata();
	Gun->Updata();
	PartRed->Update({ 1.0f,0.0f,0.0f,0.0f });
	PartGreen->Update({ 0.0f,1.0f,0,0.0f });
}

void Player::ParticleDraw(ID3D12GraphicsCommandList* cmdeList)
{
	ParticleManager::PreDraw(cmdeList);
	PartRed->Draw();
	PartGreen->Draw();
	ParticleManager::PostDraw();
}

void Player::SpriteDraw()
{
	if (CamWork == true) {
		SpriteRet->Draw();
	}
	else {
		Curtain_Up->Draw();
		Curtain_Down->Draw();
		Skip->Draw();
	}
}

void Player::CameraWork()
{
	if (CamWork == false && Start_F == false) {

		if (stanby == false) {
			Eye_rot.y = 180;
		}
		else if (stanby == true && ActionCount == 0) {
			Eye_rot.y -= 2;
			//後ろを向く
			if (Eye_rot.y <= 0) {
				Eye_rot.y = 0;
				ActionTimer += 0.2f;
				if (ActionTimer > 5) {
					ActionTimer = 5.0f;
					Eye_rot.x += 1.0f;
				}
			}
			//下を向く
			if (Eye_rot.x >= 90) {
				ActionTimer = 0.0f;
				Eye_rot.x = 90;
				ActionCount = 1;
			}
		}
		if (ActionCount == 1) {

			ActionTimer += 0.15f;
			vel = { 0.0f,0.67f,0.4f };
			if (ActionTimer >= 5) {
				vel = { 0.0f,-0.6f,0.0f };
				Eye_rot.x -= 0.7f;
			}
			if (Eye_rot.x <= 0.0f) {
				Eye_rot.x = 0.0f;
			}
			if (PlayerWorld_Pos.m128_f32[1] <= 0.3f) {
				vel = { 0.0f,0.0f,0.0f };
				Ret_Pos.m128_f32[1] = 0.0f;
				MovieFlag = true;
			}
		}

	}

	if ((Mouse::GetInstance()->PushClick(1) || Mouse::GetInstance()->PushClick(0)) && stanby == true && CamWork == false) {
		MovieFlag = true;
		ActionCount = 100;
		Eye_rot.x = 0;
		Eye_rot.y = 0;
		Ret_Pos = { 0.0f,-0.7f,13.0f };
		Railcam->Initialize(Ret_Pos, Eye_rot);
	}

	if (stanby == false) {
		ActionTimer += 0.01f;
		if (ActionTimer >= 1.0f) {
			ActionTimer = 0.0f;
			stanby = true;
		}
	}

	if (MovieFlag == false) {
		curtainPos.y += 4;
		curtainPos2.y -= 4;
		skipPos.y -= 2;

		if (curtainPos.y >= 0) {
			curtainPos.y = 0;
		}

		if (curtainPos2.y <= 620) {
			curtainPos2.y = 620;
		}

		if (skipPos.y <= 620) {
			skipPos.y = 620;
		}
	}

	else {
		curtainPos.y -= 4;
		curtainPos2.y += 4;
		skipPos.y += 4;

		if (curtainPos.y <= -100) {
			curtainPos.y = -100;
		}

		if (curtainPos2.y >= 720) {
			curtainPos2.y = 720;
			CamWork = true;
			Start_F = true;
		}

		if (skipPos.y >= 720) {
			skipPos.y = 12000;
		}
	}
	Curtain_Up->SetPosition(curtainPos);
	Curtain_Down->SetPosition(curtainPos2);
	Skip->SetPosition(skipPos);
}

void Player::PlayerMove(bool& move, int patern)
{
	XMMATRIX camMat = Railcam->GetWorld();
	XMVECTOR camvec = { 0.0f,0.0f,0.0f,0.0f };

	camvec = XMVector3Transform(camvec, camMat);

	PaternCount = patern;
	//敵をすべて倒した時
	if (move == true) {
		Active = true;
	}

	if (Active == true) {
		kBulletSpeed = 0.5f;
		if (ShakeHead_F == true) {
			if (shake == 0) {
				Eye_rot.x += 0.05f;
				if (Eye_rot.x >= 0.5f) {
					shake = 1;
				}
			}
			if (shake == 1) {
				Eye_rot.x -= 0.05f;
				if (Eye_rot.x <= -0.5f) {
					shake = 0;
				}
			}
		}
		if (patern == 0) {
			vel = { 0, 0, kBulletSpeed };
			EneCount = 3;
			if (camvec.m128_f32[2] >= 20) {
				move = false;
				Active = false;
				WaveCount += 1;
				MoveTimer = 0.0f;
				Finish = true;
			}
			else {
				Distance = 11;
			}
		}
		else if (patern == 1) {
			EneCount = 4;
			vel = { 0, 0, kBulletSpeed };
			if (camvec.m128_f32[2] >= 40) {
				move = false;
				Active = false;
				WaveCount += 1;
				MoveTimer = 0.0f;
				Finish = true;
			}
			else {
				Distance = 11;
			}
		}
		else if (patern == 2) {
			EneCount = 4;
			Eye_rot.y += 3;
			if (Eye_rot.y >= 90) {
				Eye_rot.y = 90;
				ChangeRot = -90;
				vel = { 0, 0, kBulletSpeed };
			}
			if (camvec.m128_f32[0] >= 30) {
				move = false;
				Active = false;
				WaveCount += 1;
				MoveTimer = 0.0f;
				Finish = true;
			}
			else {
				Distance = 11;
			}
		}
		else if (patern == 3) {
			EneCount = 3;
			vel = { 0, 0, kBulletSpeed };

			if (camvec.m128_f32[0] >= 45) {
				move = false;
				Active = false;
				WaveCount += 1;
				MoveTimer = 0.0f;
				Finish = true;
			}
			else {
				Distance = 11;
			}
		}
		else if (patern == 4) {
			EneCount = 5;
			Eye_rot.y -= 3;
			if (Eye_rot.y <= 0) {
				ChangeRot = 0;
				Eye_rot.y = 0;
				vel = { 0, 0, kBulletSpeed };
			}
			if (camvec.m128_f32[2] >= 70) {
				move = false;
				Active = false;
				WaveCount += 1;
				MoveTimer = 0.0f;
				Finish = true;
			}
			else {
				Distance = 11;
			}
		}
		else if (patern == 5) {
			Eye_rot.y -= 3;
			if (Eye_rot.y <= 0) {
				Eye_rot.y = 0;
				vel = { 0, 0, kBulletSpeed };
			}
			if (camvec.m128_f32[2] >= 90) {
				move = false;
				Active = false;
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
						vel = { 0.0f,0.674f,0.0f };
					}
				}
			}
			move = false;
			CamWork = false;

			WaveCount += 1;
			MoveTimer = 0.0f;
			MovieFlag = false;
			ActionCount = 0;
		}
	}
}

void Player::ObjDraw()
{
	Track->Draw();
	if (Hp >= 0 && CamWork == true) {
		Gun->Draw();
	}
}

void Player::ImGuiDraw()
{

	XMMATRIX camMat = Railcam->GetWorld();
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


	if (ImGui::TreeNode("Ret_Rot")) {
		ImGui::SliderFloat("Rot.x", &Ret_Rot.x, -100.0f, 100.0f);
		ImGui::SliderFloat("Rot.y", &Ret_Rot.y, -100.0f, 100.0f);
		ImGui::SliderFloat("Rot.z", &Ret_Rot.z, -100.0f, 100.0f);
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("retpos")) {
		ImGui::SliderFloat("retpos.x", &retpos.x, 0.0f, 1280.0f);
		ImGui::SliderFloat("retpos.y", &retpos.y, 0.0f, 720.0f);
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("gunpos")) {
		ImGui::SliderFloat("gunpos.x", &Gun_Pos.m128_f32[0], -100.0f, 100.0f);
		ImGui::SliderFloat("gunpos.y", &Gun_Pos.m128_f32[1], -100.0f, 100.0f);
		ImGui::SliderFloat("gunpos.z", &Gun_Pos.m128_f32[2], -100.0f, 100.0f);
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("gunWorldpos")) {
		ImGui::SliderFloat("gunWordpos.x", &GunWorld_Pos.m128_f32[0], -100.0f, 100.0f);
		ImGui::SliderFloat("gunWordpos.y", &GunWorld_Pos.m128_f32[1], -100.0f, 100.0f);
		ImGui::SliderFloat("gunWordpos.z", &GunWorld_Pos.m128_f32[2], -100.0f, 100.0f);
		ImGui::TreePop();
	}

	ImGui::End();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
}

void Player::Finalize()
{
	delete TrackModel;
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
	
	Shot_SE->LoadFile("Resources/Sound/SE/shot.wav", 0.3f);
}

void Player::MouthContoroll(const XMVECTOR enePos[], Camera* came, const XMFLOAT2 Ene2dPos[])
{
	Mouse::GetInstance()->MouseMoveSprite(retpos);

	SpriteRet->SetPosition(retpos);

	XMMATRIX matViewport;

	ChangeViewPort(matViewport);

	XMMATRIX View = came->GetViewMatrix();
	XMMATRIX Pro = came->GetProjectionMatrix();

	RetWorld_Pos = TrackWorld_Pos;

	Distance = 11;

	Mouse::GetInstance()->Mousemove(View, Pro, matViewport, retpos, RetWorld_Pos, Distance);


	for (int i = 0; i < EneCount; i++) {
		if ((retpos.x >= Ene2dPos[i].x - 16) && (retpos.x <= Ene2dPos[i].x + 16)) {

			RetWorld_Pos.m128_f32[0] = enePos[i].m128_f32[0];
			RetWorld_Pos.m128_f32[2] = enePos[i].m128_f32[2];
			RetWorld_Pos.m128_f32[1] = RetWorld_Pos.m128_f32[1] - 0.35f;
		}
	}

	Track->SetPosition(RetWorld_Pos);


}

void Player::ParticleEfect()
{
	if (particle == true) {
		for (int i = 0; i < 10; i++) {
			double radX = Ret_Rot.y * M_PI / 180;
			double radY = Gun_Rot.x * M_PI / 180;
			float sinradX = sinf(radX);
			float cosradX = cosf(radX);

			float sinradY = sinf(radY);
			float cosradY = cosf(radY);
			if (PaternCount == 3 || PaternCount == 4) {
				pos.x = GunWorld_Pos.m128_f32[0] + 2.3f;
				pos.y = GunWorld_Pos.m128_f32[1] - sinradY * 3;
				pos.z = GunWorld_Pos.m128_f32[2]-2.8f*cosradX;
			}
			else if (PaternCount == 0 || PaternCount == 1 || PaternCount == 2 || PaternCount == 5) {
				pos.x = GunWorld_Pos.m128_f32[0]+sinradX*2.8f;
				pos.y = GunWorld_Pos.m128_f32[1]-sinradY*3;
				pos.z = GunWorld_Pos.m128_f32[2] + 2.3f;
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
		particle = false;
		SoundEffect();
	}

}


