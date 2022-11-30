#include "Player.h"
#include<cassert>
#include "imgui/imgui.h"
#include"imgui/imconfig.h"
#include"WinApp.h"

void Player::Initalize()
{
	camera = new Camera(WinApp::window_width, WinApp::window_height);
	Object3d::SetCamera(camera);

	TrackModel = ObjModel::CreateFromOBJ("block0");
	Track = Object3d::Create(TrackModel);

	gunModel = ObjModel::CreateFromOBJ("gun");
	gun = Object3d::Create(gunModel);

	reticleModel = ObjModel::CreateFromOBJ("block0");
	reticle = Object3d::Create(reticleModel);

	playerModel = ObjModel::CreateFromOBJ("block0");
	player = Object3d::Create(playerModel);

	part = ParticleManager::Create();

	Sprite::LoadTexture(200, L"Resources/mark.png");
	spriteRet.reset(Sprite::SpriteCreate(200, retpos, spCol, anc));

	cam = new RailCamera();
	cam->Initialize(position, rotation);

	player->SetParent(camera);

};

void Player::Set()
{
	const float BulletSpeed = 15.0f;
	velocity = { 0, 0, BulletSpeed };
	velocity = XMVector3TransformNormal(velocity, mat);
	TrackWorldPos = { 0.0f,0.0f,2.0f };
	TrackWorldPos = XMVector3Transform(TrackWorldPos, mat);

	gunmat = gun->GetMatrix();
	gunWorldPos = { 0.0f,0.0f,0.0f };
	gunWorldPos = XMVector3Transform(gunWorldPos, gunmat);
	gunWorldPos.m128_f32[1] = TrackWorldPos.m128_f32[1];

	playermat = player->GetMatrix();
	playerWorldPos = { 0.0f,0.0f,0.0f };
	playerWorldPos = XMVector3Transform(playerWorldPos, playermat);

	for (int i = 0; i < 100; i++) {
		const float rnd_pos = 1.0f;
		XMFLOAT3 pos{};
		pos.x = TrackWorldPos.m128_f32[0];
		pos.y = TrackWorldPos.m128_f32[1];
		pos.z = TrackWorldPos.m128_f32[2];
		//pos.x = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		//pos.y = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		//pos.z = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;

		const float rnd_vel = 0.001f;
		XMFLOAT3 vel{};
		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

		XMFLOAT3 acc{};
		const float rnd_acc = 0.001f;
		acc.y = -(float)rand() / RAND_MAX * rnd_acc;
		if (time < 20.0f) {
			part->Add(10, pos, vel, acc, 0.5f, 0.0f, time);
		}
		break;
	}

	//レティクルObj
	Track->SetRotation({ rotation });
	Track->SetScale({ scale });
	mat = Track->GetMatrix();

	//プレイヤーObj
	player->SetRotation(playerRot);
	player->SetPosition(playerPos);
	player->SetScale(playerScl);
	player->SetParent(camera);
	

	gun->SetRotation(gunRot);
	gun->SetScale(gunScal);
	gun->SetParent(camera);
	gun->SetPosition(gunPos);

}

void Player::Updata(Bullet* bull[], int& Remaining)
{

	//弾の発射前
	if (Remaining < BULL - 1 && ReloadFlag == false) {
		if (Input::GetInstance()->TriggerKey(DIK_SPACE)||Mouse::GetInstance()->PushClick(0)) {

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

	//撃った時のパーティクル
	if (particle == true) {
		time += 0.4f;
		if (time >= 3.0f) {
			particle = false;
		}
	}
	else {
		time = 4.0f;
	}
	//リロード
	if ((Input::GetInstance()->TriggerKey(DIK_R)||Mouse::GetInstance()->PushClick(1)) && Remaining != 0) {
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
	//プレイヤーの移動
	Action::GetInstance()->PlayerMove3d(position);
	const float kMoveLimitX = 4;
	const float kMoveLimitY = 3;

	position.m128_f32[0] = max(position.m128_f32[0], -kMoveLimitX);
	position.m128_f32[0] = min(position.m128_f32[0], +kMoveLimitX);
	position.m128_f32[1] = max(position.m128_f32[1], -kMoveLimitY + 3);
	position.m128_f32[1] = min(position.m128_f32[1], +kMoveLimitY);

	if (Active == false) {
		kBulletSpeed = 0.0f;
		vel = { 0, 0, kBulletSpeed };
		Eye_rot.x = 0;
	}

	vel = XMVector3TransformNormal(vel, playermat);

	for (int i = 0; i < 9; i++) {
		bull[i]->Updata();
	}


	//ReteicleHaiti();
	MouthContoroll();
	
	Set();
	cam->Updata(vel, Eye_rot, camera);
	camera->Updata();
	Track->Updata();
	player->Updata();
	gun->Updata();
}

void Player::ParticleDraw(ID3D12GraphicsCommandList* cmdeList)
{
	ParticleManager::PreDraw(cmdeList);
	if (particle == true) {
		part->Draw();
	}
	ParticleManager::PostDraw();
}

void Player::SpriteDraw()
{
	spriteRet->Draw();
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

		if (patern == 0) {
			vel = { 0, 0, kBulletSpeed };
			if (camvec.m128_f32[2] >= 20) {
				move = false;
				Active = false;
				waveCount += 1;
				movetimer = 0.0f;
			}
		}
		else if (patern == 1) {
			vel = { 0, 0, kBulletSpeed };
			if (camvec.m128_f32[2] >= 40) {
				move = false;
				Active = false;
				waveCount += 1;
				movetimer = 0.0f;
			}
		}
		else if (patern == 2) {
			Eye_rot.y += 3;
			rotation.y -= 3;
			if (Eye_rot.y >= 90) {
				Eye_rot.y = 90;
				rotation.y = -90;
				vel = { 0, 0, kBulletSpeed };
			}
			if (camvec.m128_f32[0] >= 30) {
				move = false;
				Active = false;
				waveCount += 1;
				movetimer = 0.0f;
			}
		}
		else if (patern == 3) {

			vel = { 0, 0, kBulletSpeed };

			if (camvec.m128_f32[0] >= 45) {
				move = false;
				Active = false;
				waveCount += 1;
				movetimer = 0.0f;
			}
		}
		else if (patern == 4) {
			Eye_rot.y -= 3;
			rotation.y += 3;
			if (Eye_rot.y <= 0) {
				rotation.y = 0;
				Eye_rot.y = 0;
				vel = { 0, 0, kBulletSpeed };
			}
			if (camvec.m128_f32[2] >= 70) {
				move = false;
				Active = false;
				waveCount += 1;
				movetimer = 0.0f;
			}
		}
		else if (patern == 5) {
			Eye_rot.y -= 3;
			if (Eye_rot.y <= 0) {
				Eye_rot.y = 0;
				vel = { 0, 0, kBulletSpeed };
			}
			if (camvec.m128_f32[2] <= 0) {
				move = false;
				Active = false;
				waveCount += 1;
				movetimer = 0.0f;
			}
		}
		else if (patern == 6) {
			Eye_rot.y -= 3;
			if (Eye_rot.y >= 270) {
				Eye_rot.y = 270;
				vel = { 0, 0, kBulletSpeed };
			}
			if (camvec.m128_f32[0] <= 20) {
				move = false;
				Active = false;
				waveCount += 1;
				movetimer = 0.0f;
			}
		}
		else if (patern == 8) {
			vel = { 0, 0, kBulletSpeed };
			if (camvec.m128_f32[0] <= 0) {
				kBulletSpeed = 0.0f;
				vel = { 0, 0, kBulletSpeed };
				Eye_rot.y += 3;
				if (Eye_rot.y >= 360) {
					Eye_rot.y = 360;
					move = false;
					Active = false;
					waveCount += 1;
					movetimer = 0.0f;
				}
			}
		}
	}
}

void Player::ObjDraw()
{
	cam->Draw();
	if (Hp >= 0) {
		Track->Draw();
		gun->Draw();
	}
}

void Player::ImGuiDraw()
{
	float a = waveCount;

	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.7f, 0.7f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.1f, 0.0f, 0.1f, 0.0f));
	ImGui::SetWindowSize(ImVec2(400, 500), ImGuiCond_::ImGuiCond_FirstUseEver);
	ImGui::Begin("Plyer");

	if (ImGui::TreeNode("LoacalPos")) {
		ImGui::SliderFloat("pos.x", &position.m128_f32[0], -100.0f, 100.0f);
		ImGui::SliderFloat("pos.y", &position.m128_f32[1], -100.0f, 100.0f);
		ImGui::SliderFloat("pos.z", &position.m128_f32[2], -100.0f, 100.0f);
		ImGui::SliderFloat("mat.x", &a, -100.0f, 100.0f);
		ImGui::TreePop();
	}



	if (ImGui::TreeNode("TrackWorldPos")) {
		ImGui::SliderFloat("pos.x", &TrackWorldPos.m128_f32[0], -100.0f, 100.0f);
		ImGui::SliderFloat("pos.y", &TrackWorldPos.m128_f32[1], -100.0f, 100.0f);
		ImGui::SliderFloat("pos.z", &TrackWorldPos.m128_f32[2], -100.0f, 100.0f);
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("playerRot")) {
		ImGui::SliderFloat("Rot.x", &Eye_rot.x, -100.0f, 100.0f);
		ImGui::SliderFloat("Rot.y", &Eye_rot.y, -100.0f, 100.0f);
		ImGui::SliderFloat("Rot.z", &Eye_rot.z, -100.0f, 100.0f);
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

void Player::MouthContoroll()
{
	Mouse::GetInstance()->MouseMoveSprite(retpos);

	spriteRet->SetPosition(retpos);

	XMMATRIX matViewport;

	ChangeViewPort(matViewport);

	XMMATRIX View = camera->GetViewMatrix();
	XMMATRIX Pro = camera->GetProjectionMatrix();

	XMVECTOR positionRet = TrackWorldPos;

	Mouse::GetInstance()->Mousemove(View,Pro, matViewport, retpos, positionRet);

	Track->SetPosition(positionRet);
}


