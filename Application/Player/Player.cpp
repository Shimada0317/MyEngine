#include "Player.h"
#include<cassert>
#include "imgui/imgui.h"
#include"imgui/imconfig.h"
#include"WinApp.h"

Player::~Player()
{
	delete TrackModel;
	delete gunModel;
	delete playerModel;

	Track.reset();
	gun.reset();
	player.reset();
}

void Player::Initalize()
{
	camera = new Camera(WinApp::window_width, WinApp::window_height);
	Object3d::SetCamera(camera);

	TrackModel = ObjModel::CreateFromOBJ("tst2");
	Track = Object3d::Create(TrackModel);

	gunModel = ObjModel::CreateFromOBJ("gun");
	gun = Object3d::Create(gunModel);

	playerModel = ObjModel::CreateFromOBJ("gun");
	player = Object3d::Create(playerModel);

	Sprite::LoadTexture(200, L"Resources/mark.png");
	spriteRet.reset(Sprite::SpriteCreate(200, retpos, spCol, anc));

	Sprite::LoadTexture(300, L"Resources/curtain.png");
	curtain.reset(Sprite::SpriteCreate(300, curtainPos));

	Sprite::LoadTexture(301, L"Resources/window.png");
	window.reset(Sprite::SpriteCreate(301, windowPos));

	skipPos = curtainPos2;

	Sprite::LoadTexture(302, L"Resources/skip.png");
	skip.reset(Sprite::SpriteCreate(302, skipPos));

	curtain2.reset(Sprite::SpriteCreate(300, curtainPos2));
	curtain->SetSize(curtainSiz);
	curtain2->SetSize(curtainSiz);

	cam = new RailCamera();
	cam->Initialize(position, rotation);

	player->SetParent(camera);

	part = ParticleManager::Create(camera);
};

void Player::Set()
{
	const float BulletSpeed = 15.0f;
	velocity = { 0, 0, BulletSpeed };
	velocity = XMVector3TransformNormal(velocity, mat);
	TrackWorldPos = { 0.0f,0.0f,2.0f };
	TrackWorldPos = XMVector3Transform(TrackWorldPos, mat);

	

	WorldFarPos = TrackWorldPos;

	playermat = player->GetMatrix();
	playerWorldPos = { -10.0f,0.0f,-20.0f };
	playerWorldPos = XMVector3Transform(playerWorldPos, playermat);

	
	/*if (retpos.x <= 0) {
		rotation.y = rotation.y  20;
	}

	if (retpos.x >= 1280) {
		rotation.y = 20;
	}

	if (retpos.x == WinApp::window_width) {
		rotation.y = 0;
	}*/

	rotation.y = (retpos.x - 640)/10+changeRot;
	//rotation.x = -(retpos.y - 360) / 10;
	//レティクルObj
	Track->SetRotation({ rotation });
	Track->SetScale({ scale });
	mat = Track->GetMatrix();

	//プレイヤーObj
	player->SetRotation(playerRot);
	player->SetPosition(playerPos);
	player->SetScale(playerScl);
	player->SetParent(camera);

	gunRot.y= (retpos.x - 640) / 10;
	gunRot.x = (retpos.y - 360) / 50 ;

	
	gun->SetRotation(gunRot);
	gun->SetScale(gunScal);
	gun->SetParent(camera);
	gun->SetPosition(gunPos);

	gunmat = gun->GetMatrix();
	gunWorldPos = { 0.0f,0.0f,0.0f };
	gunWorldPos = XMVector3Transform(gunWorldPos, gunmat);
	//gunWorldPos.m128_f32[1] = TrackWorldPos.m128_f32[1];

	if (particle == true) {
		for (int i = 0; i < 10; i++) {
			const float rnd_pos = 0.0f;
			XMFLOAT3 pos;
			pos.x = gunWorldPos.m128_f32[0];
			pos.y = gunWorldPos.m128_f32[1];
			pos.z = gunWorldPos.m128_f32[2]+1.5f;
			const float rnd_vel = 0.001f;
			XMFLOAT3 vel{};
			vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
			vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
			vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

			XMFLOAT3 acc{};
			acc.y = -0.00001;
			acc.y = -0.00001;
			//if (Input::GetInstance()->PushClick(1)) {
			part->Add(10, pos, vel, acc, 0.5f, 0.2f, time);
			//}
		}
		particle = false;
	}
}

void Player::Updata(Bullet* bull[], int& Remaining, const XMVECTOR enePos[])
{

	if (CamWork == true) {
		//弾の発射前
		if (Remaining < BULL - 1 && ReloadFlag == false) {
			if (Mouse::GetInstance()->PushClick(0)) {

				Remaining += 1;
				time = 0.0f;
				particle = true;
				for (int i = 0; i < BULL; i++) {
					if (bull[i]->CheckOk()) {
						bull[i]->Test(gunWorldPos, TrackWorldPos, Eye_rot);
						bull[i]->TriggerOn();
						break;
					}
				}
			}
		}

		//リロード
		if ((Mouse::GetInstance()->PushClick(1)) && Remaining != 0) {
			ReloadFlag = true;
		}

		if (ReloadFlag == true) {
			ReloadTime += 1;
			ans = ReloadTime % 10;
			if (ans == 0) {
				Remaining -= 1;
				if (Remaining == 0) {
					ReloadFlag = false;
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

	vel = XMVector3TransformNormal(vel, playermat);

	for (int i = 0; i < 9; i++) {
		bull[i]->Updata();
	}

	MouthContoroll(enePos);

	CameraWork();

	Set();
	cam->Updata(vel, Eye_rot, camera);
	camera->Updata();
	Track->Updata();
	player->Updata();
	gun->Updata();
	part->Update({1.7f,0.5f,0,0.5f});
}

void Player::ParticleDraw(ID3D12GraphicsCommandList* cmdeList)
{
	ParticleManager::PreDraw(cmdeList);
	part->Draw();
	ParticleManager::PostDraw();
}

void Player::SpriteDraw()
{
	if (CamWork == true) {
		spriteRet->Draw();
	}
	else {
		curtain->Draw();
		curtain2->Draw();
		skip->Draw();
	}
}

void Player::CameraWork()
{
	if (CamWork == false && start == false) {

		if (stanby == false) {
			Eye_rot.y = 180;
		}
		else if (stanby == true && act == 0) {
			Eye_rot.y -= 2;

			if (Eye_rot.y <= 0) {
				Eye_rot.y = 0;
				actTime += 0.2f;
				if (actTime > 5) {
					actTime = 5.0f;
					Eye_rot.x += 1.0f;
				}
			}
			if (Eye_rot.x >= 90) {
				actTime = 0.0f;
				Eye_rot.x = 90;
				act = 1;
			}
		}
		if (act == 1) {

			actTime += 0.15f;
			vel = { 0.0f,0.6f,0.4f };
			if (actTime >= 5) {
				vel = { 0.0f,-0.6f,0.0f };
				Eye_rot.x -= 0.7f;
			}
			if (Eye_rot.x <= 0.0f) {
				Eye_rot.x = 0.0f;
			}
			if (playerWorldPos.m128_f32[1] <= 0.3f) {
				vel = { 0.0f,0.0f,0.0f };
				position.m128_f32[1] = 0.0f;
				a = true;
			}
		}

	}

	if ((Input::GetInstance()->PushClick(1) || Input::GetInstance()->PushClick(0)) && stanby == true&& CamWork ==false) {
		a = true;
		act = 100;
		Eye_rot.x = 0;
		Eye_rot.y = 0;
		position = { 0.0f,-0.7f,13.0f };
		cam->Initialize(position, Eye_rot);
	}

	if (stanby == false) {
		actTime += 0.01f;
		if (actTime >= 1.0f) {
			actTime = 0.0f;
			stanby = true;
		}
	}

	if (a == false) {
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
			start = true;
		}

		if (skipPos.y >= 720) {
			skipPos.y = 12000;
		}
	}
	curtain->SetPosition(curtainPos);
	curtain2->SetPosition(curtainPos2);
	skip->SetPosition(skipPos);
}

void Player::PlayerMove(bool& move, int patern)
{
	XMMATRIX camMat = cam->GetWorld();
	XMVECTOR camvec = { 0.0f,0.0f,0.0f,0.0f };

	camvec = XMVector3Transform(camvec, camMat);


	//敵をすべて倒した時
	if (move == true) {
		Active = true;
	}

	if (Active == true) {
		kBulletSpeed = 0.5f;
		if (Shake == true) {
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
				waveCount += 1;
				movetimer = 0.0f;
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
				waveCount += 1;
				movetimer = 0.0f;
				Finish = true;
			}
			else {
				Distance = 11;
			}
		}
		else if (patern == 2) {
			EneCount = 4;
			Eye_rot.y += 3;
			rotation.y -= 3;
			if (Eye_rot.y >= 90) {
				Eye_rot.y = 90;
				changeRot = -90;
				rotation.y = changeRot;
				vel = { 0, 0, kBulletSpeed };
			}
			if (camvec.m128_f32[0] >= 30) {
				move = false;
				Active = false;
				waveCount += 1;
				movetimer = 0.0f;
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
				waveCount += 1;
				movetimer = 0.0f;
				Finish = true;
			}
			else {
				Distance = 11;
			}
		}
		else if (patern == 4) {
			EneCount = 5;
			Eye_rot.y -= 3;
			rotation.y += 3;
			if (Eye_rot.y <= 0) {
				changeRot = 0;
				rotation.y = changeRot;
				Eye_rot.y = 0;
				vel = { 0, 0, kBulletSpeed };
			}
			if (camvec.m128_f32[2] >= 70) {
				move = false;
				Active = false;
				waveCount += 1;
				movetimer = 0.0f;
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
				waveCount += 1;
				movetimer = 0.0f;
				Finish = true;
			}
			else {
				Distance = 11;
			}
		}
		else if (patern == 6) {
			stanby = false;
			vel = { 0, 0, 0.1 };
			Shake = false;
			if (camvec.m128_f32[2] >= 92) {
				vel = { 0,0.05,0.1 };
				if (camvec.m128_f32[2] >= 97) {
					vel = { 0.0f,0.0f,0.0f };
					Fring = true;
					if (Fring == true) {
						vel = { 0.0f,0.670f,0.0f };
					}
				}
			}
			move = false;
			CamWork = false;

			waveCount += 1;
			movetimer = 0.0f;
			//Finish = true;
			a = false;
			act = 0;
		}
	}
}

void Player::ObjDraw()
{
	Track->Draw();
	if (Hp >= 0 && CamWork == true) {
		gun->Draw();
	}
}

void Player::ImGuiDraw()
{
	float a = waveCount;

	XMMATRIX camMat = cam->GetWorld();
	XMVECTOR camvec = { 0.0f,0.0f,0.0f,0.0f };

	camvec = XMVector3Transform(camvec, camMat);

	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.7f, 0.7f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.1f, 0.0f, 0.1f, 0.0f));
	ImGui::SetWindowSize(ImVec2(400, 500), ImGuiCond_::ImGuiCond_FirstUseEver);
	ImGui::Begin("Plyer");

	if (ImGui::TreeNode("LoacalPos")) {
		ImGui::SliderFloat("pos.x", &positionRet.m128_f32[0], -100.0f, 100.0f);
		ImGui::SliderFloat("pos.y", &positionRet.m128_f32[1], -100.0f, 100.0f);
		ImGui::SliderFloat("pos.z", &positionRet.m128_f32[2], -100.0f, 100.0f);
		ImGui::SliderFloat("mat.x", &a, -100.0f, 100.0f);
		ImGui::TreePop();
	}


	if (ImGui::TreeNode("rotation")) {
		ImGui::SliderFloat("Rot.x", &rotation.x, -100.0f, 100.0f);
		ImGui::SliderFloat("Rot.y", &rotation.y, -100.0f, 100.0f);
		ImGui::SliderFloat("Rot.z", &rotation.z, -100.0f, 100.0f);
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("retpos")) {
		ImGui::SliderFloat("retpos.x", &retpos.x, 0.0f, 1280.0f);
		ImGui::SliderFloat("retpos.y", &retpos.y, 0.0f, 720.0f);
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("gunpos")) {
		ImGui::SliderFloat("gunpos.x", &gunPos.m128_f32[0], -100.0f, 100.0f);
		ImGui::SliderFloat("gunpos.y", &gunPos.m128_f32[1], -100.0f, 100.0f);
		ImGui::SliderFloat("gunpos.z", &gunPos.m128_f32[2], -100.0f, 100.0f);
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("gunWorldpos")) {
		ImGui::SliderFloat("gunWordpos.x", &gunWorldPos.m128_f32[0], -100.0f, 100.0f);
		ImGui::SliderFloat("gunWordpos.y", &gunWorldPos.m128_f32[1], -100.0f, 100.0f);
		ImGui::SliderFloat("gunWordpos.z", &gunWorldPos.m128_f32[2], -100.0f, 100.0f);
		ImGui::TreePop();
	}



	ImGui::End();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
}

void Player::Finalize()
{
	delete camera;
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

void Player::ReteicleHaiti()
{
	const float kDistancePlayerTo3DReticle = 50.0f;

	offset = { 0,0,1.0f };

	offset = XMVector3TransformNormal(offset, mat);

	offset = XMVector3Normalize(offset) * kDistancePlayerTo3DReticle;



	{
		XMVECTOR positionRet = TrackWorldPos;

		ChangeViewPort(matViewPort);

		XMMATRIX matVP = matViewPort;

		XMMATRIX GetViewPro = camera->GetViewProjectionMatrix();

		XMMATRIX matViewProjectionViewport = GetViewPro * matVP;

		positionRet = XMVector3TransformCoord(positionRet, matViewProjectionViewport);

		spriteRet->SetPosition({ positionRet.m128_f32[0],positionRet.m128_f32[1] });
	}
}

void Player::MouthContoroll(const XMVECTOR enePos[])
{
	Mouse::GetInstance()->MouseMoveSprite(retpos);

	spriteRet->SetPosition(retpos);

	XMMATRIX matViewport;

	ChangeViewPort(matViewport);

	XMMATRIX View = camera->GetViewMatrix();
	XMMATRIX Pro = camera->GetProjectionMatrix();

	positionRet = TrackWorldPos;


	Distance = 11;
	

	Mouse::GetInstance()->Mousemove(View, Pro, matViewport, retpos, positionRet,Distance);

	for (int i = 0; i < EneCount; i++) {
		if ((positionRet.m128_f32[0] >= enePos[i].m128_f32[0] - 0.5f && positionRet.m128_f32[0] <= enePos[i].m128_f32[0] + 0.5f)|| (positionRet.m128_f32[2] >= enePos[i].m128_f32[2] - 0.5f && positionRet.m128_f32[2] <= enePos[i].m128_f32[2] + 0.5f)) {
			positionRet = enePos[i];
		}
	}

	Track->SetPosition(positionRet);

	
}


