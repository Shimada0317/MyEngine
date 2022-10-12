#include "Player.h"
#include<cassert>
#include "imgui/imgui.h"
#include"imgui/imconfig.h"
#include"WinApp.h"

void Player::Initalize()
{
	camera = new DebugCamera(WinApp::window_width, WinApp::window_height);
	Object3d::SetCamera(camera);

	playerModel = ObjModel::CreateFromOBJ("mark");
	player = Object3d::Create(playerModel);

	input = Input::GetInstance();
	debugtext = DebugText::GetInstance();

	//for (int i = 0; i < BULL; i++) {

		//bull->Initialize();
	//}

	part = ParticleManager::Create();
	backPlayerPos.m128_f32[0] = -position.m128_f32[0] / 32;
	backPlayerPos.m128_f32[1] = position.m128_f32[1] / 32;
	backPlayerPos.m128_f32[2] = position.m128_f32[2] - 5;

	cam = new RailCamera();
	cam->Initialize(position,rotation);

};

void Player::Set()
{
	backPlayerPos.m128_f32[0] = -position.m128_f32[0] / 32;
	backPlayerPos.m128_f32[1] = position.m128_f32[1] / 32;
	backPlayerPos.m128_f32[2] = position.m128_f32[2] - 5;

	for (int i = 0; i < 100; i++) {
		const float rnd_pos = 1.0f;
		XMFLOAT3 pos{};
		pos.x = oldPos.m128_f32[0];
		pos.y = oldPos.m128_f32[1];
		pos.z = oldPos.m128_f32[2];
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
	if (patern == true) {
		position = player->GetPosition();
	}
	player->SetPosition({ position });
	player->SetRotation({ rotation });
	player->SetScale({ scale });
	Target_pos = camera->GetTarget();
	if (ver == 1) {
		Target_pos.x += 0.11f;
	}
	else if (ver == 2) {
		Target_pos.z -= position.m128_f32[2] + 100;
	}

	Target_pos=cam->GetTarget();
	Eye_pos = cam->Getye();

	camera->SetTarget({ Target_pos.x,Target_pos.y,position.m128_f32[2] });
	camera->SetEye({ Eye_pos });
	camera->SetDistance(5);


}

void Player::Effect()
{
}

void Player::Update(Bullet* bull[], int& Remaining)
{
	XMVECTOR veloc = { 0.0f,0.0f,0.1f };
	cam->Update(veloc,rotation);
	//if (Remaining < BULL - 1 && ReloadFlag == false) {
	//	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
	//		Remaining += 1;
	//		oldPos = position;
	//		time = 0.0f;
	//		particle = true;
	//		for (int i = 0; i < BULL; i++) {
	//			if (bull[i]->CheckOk()) {
	//				bull[i]->TriggerOn();
	//				break;
	//			}
	//		}

	//	}
	//}
	if (Remaining < BULL - 1 && ReloadFlag == false) {
		if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
			const float kBulletSpeed = 20.0f;
			XMVECTOR velocity = { 0, 0, kBulletSpeed };
			mat = player->GetMatrix();
			velocity = XMVector3TransformNormal(velocity, mat);
			Remaining += 1;
			oldPos = position;
			time = 0.0f;
			particle = true;
			for (int i = 0; i < BULL; i++) {
				if (bull[i]->CheckOk()) {
					bull[i]->TriggerOn();
					bull[i]->Test(position, velocity);
					break;
				}
			}
		}
	}
	for (int i = 0; i < BULL; i++) {
		bull[i]->ShotBefore(backPlayerPos);
		//break;
	}
	if (particle == true) {
		time += 0.4f;
		if (time >= 3.0f) {
			particle = false;

		}
	}
	else {
		time = 4.0f;
	}
	//for (int i = 0; i < BULL; i++) {
	//	bull[i]->ShotAfter(backPlayerPos, position, Remaining);
	//	//break;
	//}

	if (Input::GetInstance()->PushKey(DIK_L)) {
		position.m128_f32[2] += 0.1f;
	}

	if (Input::GetInstance()->TriggerKey(DIK_R)) {
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

	Action::GetInstance()->PlayerMove3d(position);
	//	if (Input::GetInstance()->PushKey(DIK_Z)) {
	//		Eye_pos.x += 0.1f;
	//	}
	//camera->MoveEyeVector(position);

	const float kMoveLimitX = 4;
	const float kMoveLimitY = 2;

	position.m128_f32[0] = max(position.m128_f32[0], -kMoveLimitX);
	position.m128_f32[0] = min(position.m128_f32[0], +kMoveLimitX);
	position.m128_f32[1] = max(position.m128_f32[1], -kMoveLimitY);
	position.m128_f32[1] = min(position.m128_f32[1], +kMoveLimitY);

	if (patern == false) {
		if (Input::GetInstance()->TriggerKey(DIK_O)) {
			patern = true;
		}
	}

	else {
		if (Input::GetInstance()->TriggerKey(DIK_O)) {
			patern = false;
		}
	}

	if (Input::GetInstance()->PushKey(DIK_Z)) {
		rotation.y += 0.1f;
	}

	if (patern == true) {
		const float kBulletSpeed = 1.1f;
		vel = { 0, 0, kBulletSpeed };
		mat = player->GetMatrix();
		vel = XMVector3TransformNormal(vel, mat);
	}

	/*const float kBulletSpeed = 1.1f;
	vel = { 0, 0, kBulletSpeed };
	mat = player->GetMatrix();
	vel = XMVector3TransformNormal(vel, mat);*/

	if (ver == 0) {
		if (position.m128_f32[2] >= 50) {
			rotation.y += 0.1f;
			if (rotation.y >= 90) {
				rotation.y = 90;
				chan = true;
				ver = 1;
			}
		}
	}
	else if (ver == 1) {
		if (position.m128_f32[2] >= 100) {
			rotation.y = 180;
			chan = true;
			ver = 2;
		}
	}
	else if (ver == 2) {
		if (position.m128_f32[2] >= 150) {
			rotation.y = 270;
			ver = 3;
		}
	}
	else if (ver == 3) {
		if (position.m128_f32[2] >= 200) {
			rotation.y = 0;
			position.m128_f32[2] = 0.0f;
			//ver = 0;
		}
	}

	//Attack();

	/*for (std::unique_ptr<Bullet>& bullet : bullets_) {
		bullet->Update();
	}*/

	for (int i = 0; i < 9; i++) {
		bull[i]->Update();
	}
	//MouthContoroll();
	Set();
	if (chan == true) {
		camera->Patern(ver,chan);
		chan = false;
	}
//	camera->SetVel(vel);
	camera->Update();
	player->Update(vel);
	part->Update(color);
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

void Player::ObjDraw()
{

	//for (int i = 0; i < BULL; i++) {

	//}
	cam->Draw();
	player->Draw();
}

void Player::ImGuiDraw()
{

	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.7f, 0.7f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.1f, 0.0f, 0.1f, 0.0f));
	ImGui::SetWindowSize(ImVec2(400, 500), ImGuiCond_::ImGuiCond_FirstUseEver);
	ImGui::Begin("Plyer");

	if (ImGui::TreeNode("position")) {

		ImGui::SliderFloat("rot.y", &rotation.y, -100.0f, 100.0f);
		ImGui::SliderFloat("target.x", &Target_pos.x, -100.0f, 100.0f);
		ImGui::SliderFloat("target.y", &Target_pos.y, -100.0f, 100.0f);
		ImGui::SliderFloat("Eye_pos.x", &Eye_pos.x, -100.0f, 100.0f);
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

void Player::MouthContoroll()
{

	GetCursorPos(&pos);

	HWND hwnd = WinApp::GetInstance()->GetHwnd();

	ScreenToClient(hwnd, &pos);

	retpos.x = pos.x;
	retpos.y = pos.y;

	//プロジェクション行列
	XMMATRIX matProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(60.0f),
		(float)WinApp::window_width / WinApp::window_height,
		0.1f, 1000.0f
	);
	//ビューポート行列
	XMMATRIX matViewport = { WinApp::window_width / 2,0,0,0  ,0,-WinApp::window_height / 2,0,0    ,0,0,1,0   ,WinApp::window_width / 2 + OffsetX,WinApp::window_height / 2 + OffsetY,0,1 };
	//ビュー、プロジェクション、ビューポート3つの行列の乗算
	XMMATRIX matVPV = XMMatrixLookAtLH(XMLoadFloat3(&Eye_pos), XMLoadFloat3(&cameraTarget), XMLoadFloat3(&up)) * matProjection * matViewport;

	XMMATRIX matIverserVPV = XMMatrixInverse(nullptr, matVPV);


	XMVECTOR posNear = { pos.x, pos.y, 0,1 };
	XMVECTOR posFar = { pos.x,pos.y,1,1 };

	posNear = XMVector3TransformCoord(posNear, matIverserVPV);
	posFar = XMVector3TransformCoord(posFar, matIverserVPV);

	XMVECTOR mouseDirection = posNear + posFar;
	mouseDirection = XMVector3Normalize(mouseDirection);

	const float kDistanceTestObject = 10;

	//pos=posNear+mouseDirection
}

XMVECTOR Player::GetWorldPosition()
{
	XMVECTOR worldPos;

	worldPos.m128_f32[0] = position.m128_f32[0];
	worldPos.m128_f32[1] = position.m128_f32[1];
	worldPos.m128_f32[2] = position.m128_f32[2];
	worldPos.m128_f32[3] = 1;

	return worldPos;
}
