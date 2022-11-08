#include "Player.h"
#include<cassert>
#include "imgui/imgui.h"
#include"imgui/imconfig.h"
#include"WinApp.h"

void Player::Initalize()
{
	camera = new Camera(WinApp::window_width, WinApp::window_height);
	Object3d::SetCamera(camera);

	playerModel = ObjModel::CreateFromOBJ("mark");
	player = Object3d::Create(playerModel);

	gunModel = ObjModel::CreateFromOBJ("gun");
	gun = Object3d::Create(gunModel);

	input = Input::GetInstance();
	debugtext = DebugText::GetInstance();

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
	playerWorldPos = { 0.0f,0.0f,0.0f };
	playerWorldPos = XMVector3Transform(playerWorldPos, mat);

 	gunmat = gun->GetMatrix();
	gunWorldPos = { 0.0f,0.0f,0.0f };
	gunWorldPos = XMVector3Transform(gunWorldPos, gunmat);
	gunWorldPos.m128_f32[1] = playerWorldPos.m128_f32[1];

	for (int i = 0; i < 100; i++) {
		const float rnd_pos = 1.0f;
		XMFLOAT3 pos{};
		pos.x = playerWorldPos.m128_f32[0];
		pos.y = playerWorldPos.m128_f32[1];
		pos.z = playerWorldPos.m128_f32[2];
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
	/*if (patern == true) {
		position = player->GetPosition();
	}*/
	player->SetPosition({ position });
	//player->SetRotation({ rotation });
	player->SetScale({ scale });

	mat = player->GetMatrix();
	//Eye_rot=camera->GetEye();
	player->SetParent(camera);
	gun->SetRotation(gunRot);
	gun->SetScale(gunScal);
	gun->SetParent(camera);
	gun->SetPosition(gunPos);
}

void Player::Effect()
{
}

void Player::Update(Bullet* bull[], int& Remaining)
{
	oldPos = position;

	//弾の発射前
	if (Remaining < BULL - 1 && ReloadFlag == false) {
		if (Mouse::GetInstance()->PushClick(0)) {

			Remaining += 1;
			time = 0.0f;
			particle = true;
			for (int i = 0; i < BULL; i++) {
				if (bull[i]->CheckOk()) {
					bull[i]->Test(gunWorldPos, playerWorldPos);
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
	if (Mouse::GetInstance()->PushClick(1) && Remaining != 0) {
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

	//プレイヤーの回転
	if (Input::GetInstance()->PushKey(DIK_A)) {
		Eye_rot.y++;
	}


	else if (Action == false) {
		kBulletSpeed = 0.0f;
		vel = { 0, 0, kBulletSpeed };
	}
	if (Input::GetInstance()->PushKey(DIK_O)) {
		kBulletSpeed = 1.1f;
		vel = { 0, 0, kBulletSpeed };
	}

	vel = XMVector3TransformNormal(vel, mat);

	for (int i = 0; i < 9; i++) {
		bull[i]->Update();
	}


	//ReteicleHaiti();
	MouthContoroll();
	Set();
	cam->Update(vel, Eye_rot, camera);
	camera->Update();
	player->Update();
	gun->Update();
}

void Player::ParticleDraw(ID3D12GraphicsCommandList* cmdeList)
{
	ParticleManager::PreDraw(cmdeList);
	if (particle == true) {
		part->Draw();
	}
	ParticleManager::PostDraw();
}

void Player::Draw(ID3D12GraphicsCommandList* cmdList)
{



}

void Player::SpriteDraw()
{
	spriteRet->Draw();
}

void Player::PlayerMove(bool& move, int patern, bool& spown)
{

	//敵をすべて倒した時
	if (move == true) {
		Action = true;
	}

	if (Action == true) {
		kBulletSpeed = 0.5f;
		if (patern == 1) {
			vel = { 0, 0, kBulletSpeed };
			if (playerWorldPos.m128_f32[2] >= 20) {
				move = false;
				Action = false;
				waveCount += 1;
				movetimer = 0.0f;
				spown = true;
			}
		}
		else if (patern == 2) {
			vel = { 0, 0, kBulletSpeed };
			if (playerWorldPos.m128_f32[2] >= 40) {
				move = false;
				Action = false;
				waveCount += 1;
				movetimer = 0.0f;
				spown = true;
			}
		}
		else if (patern == 3) {
			Eye_rot.y += 3;
			if (Eye_rot.y >= 90) {
				Eye_rot.y = 90;
				vel = { 0, 0, kBulletSpeed };
			}
			if (playerWorldPos.m128_f32[0] >= 20) {
				move = false;
				Action = false;
				waveCount += 1;
				movetimer = 0.0f;
				spown = true;
			}
		}
		else if (patern == 4) {
			vel = { 0, 0, kBulletSpeed };
			if (playerWorldPos.m128_f32[0] >= 40) {
				move = false;
				Action = false;
				waveCount += 1;
				movetimer = 0.0f;
				spown = true;
			}
		}
		else if (patern == 5) {
			Eye_rot.y += 3;
			if (Eye_rot.y >= 180) {
				Eye_rot.y = 180;
				vel = { 0, 0, kBulletSpeed };
			}
			if (playerWorldPos.m128_f32[2] <= 20) {
				move = false;
				Action = false;
				waveCount += 1;
				movetimer = 0.0f;
				spown = true;
			}
		}
		else if (patern == 6) {
			vel = { 0, 0, kBulletSpeed };
			if (playerWorldPos.m128_f32[2] <= 0) {
				move = false;
				Action = false;
				waveCount += 1;
				movetimer = 0.0f;
				spown = true;
			}
		}
		else if (patern == 7) {
			Eye_rot.y += 3;
			if (Eye_rot.y >= 270) {
				Eye_rot.y = 270;
				vel = { 0, 0, kBulletSpeed };
			}
			if (playerWorldPos.m128_f32[0] <= 20) {
				move = false;
				Action = false;
				waveCount += 1;
				movetimer = 0.0f;
				spown = true;
			}
		}
		else if (patern == 8) {
			vel = { 0, 0, kBulletSpeed };
			if (playerWorldPos.m128_f32[0] <= 0) {
				kBulletSpeed = 0.0f;
				vel = { 0, 0, kBulletSpeed };
				Eye_rot.y += 3;
				if (Eye_rot.y >= 360) {
					Eye_rot.y = 360;
					move = false;
					Action = false;
					waveCount += 1;
					movetimer = 0.0f;
					spown = true;
				}
			}
		}
	}
}

void Player::ObjDraw()
{
	cam->Draw();
	if (Hp >= 0) {
		player->Draw();
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

	if (ImGui::TreeNode("mat")) {
		ImGui::SliderFloat("mat.z", &mat.r[3].m128_f32[2], -100.0f, 100.0f);
		ImGui::SliderFloat("mat.y", &mat.r[3].m128_f32[1], -100.0f, 100.0f);
		ImGui::SliderFloat("mat.x", &mat.r[3].m128_f32[0], -100.0f, 100.0f);
		ImGui::SliderFloat("mat.x", &a, -100.0f, 100.0f);
		ImGui::TreePop();
	}

	

	if (ImGui::TreeNode("playerWorldPos")) {
		ImGui::SliderFloat("pos.x", &playerWorldPos.m128_f32[0], -100.0f, 100.0f);
		ImGui::SliderFloat("pos.y", &playerWorldPos.m128_f32[1], -100.0f, 100.0f);
		ImGui::SliderFloat("pos.z", &playerWorldPos.m128_f32[2], -100.0f, 100.0f);
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("gunpos")) {
		ImGui::SliderFloat("pos.x", &gunWorldPos.m128_f32[0], -100.0f, 100.0f);
		ImGui::SliderFloat("pos.y", &gunWorldPos.m128_f32[1], -100.0f, 100.0f);
		ImGui::SliderFloat("pos.z", &gunWorldPos.m128_f32[2], -100.0f, 100.0f);
		ImGui::TreePop();
	}


	if (ImGui::TreeNode("gunpos")) {
		ImGui::SliderFloat("pos.x", &gunPos.m128_f32[0], -100.0f, 100.0f);
		ImGui::SliderFloat("pos.y", &gunPos.m128_f32[1], -100.0f, 100.0f);
		ImGui::SliderFloat("pos.z", &gunPos.m128_f32[2], -100.0f, 100.0f);
		ImGui::TreePop();
	}



	ImGui::End();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
}

void Player::Finalize()
{
	delete camera;
	delete playerModel;
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

	matViewPort.r[3].m128_f32[0] = WinApp::window_width/2+offset.m128_f32[0];
	matViewPort.r[3].m128_f32[1] = WinApp::window_height/2+offset.m128_f32[1];
	matViewPort.r[3].m128_f32[2] = 0;
	matViewPort.r[3].m128_f32[3] = 1;
}

void Player::Attack()
{
	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		const float kBulletSpeed = 0.01f;
		XMVECTOR velocity = { 0, 0, kBulletSpeed,0 };
		velocity = XMVector3TransformCoord(velocity, mat);
		/*	newBullet = std::make_unique<Bullet>();
			newBullet->Initialize();
			newBullet->Stanby(position,velocity);
			bullets_.push_back(std::move(newBullet));*/
	}
}

void Player::ReteicleHaiti()
{
	const float kDistancePlayerTo3DReticle = 50.0f;

	offset = { 0,0,1.0f };

	offset = XMVector3TransformNormal(offset, mat);

	offset = XMVector3Normalize(offset) * kDistancePlayerTo3DReticle;



	{
		XMVECTOR positionRet = playerWorldPos;

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

	XMMATRIX ViewPro = camera->GetViewProjectionMatrix();

	XMVECTOR positionRet = playerWorldPos;

	Mouse::GetInstance()->Mousemove(ViewPro, matViewport, retpos, positionRet);

	player->SetPosition(positionRet);

	//pos=posNear+mouseDirection
}


