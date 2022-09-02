#include "Enemy.h"
#include<cassert>
#include"imgui/imgui.h"

void Enemy::Initalize()
{
	input = Input::GetInstance();
	debugtext = DebugText::GetInstance();
	model = ObjModel::CreateFromOBJ("enemy");
	enemy = Object3d::Create();
	enemy->SetModel(model);

}

void Enemy::Set()
{
	enemy->SetPosition(position);
	enemy->SetRotation(rotation);
	enemy->SetScale(scale);

}

void Enemy::Update(XMVECTOR pos, int wave, int oldwave, bool& StopT, XMVECTOR positionP,bool& spown)
{

	if (life <= 0) {
		arive = false;
	}
	else {
		arive = true;
	}

	if (arive == false) {
		position.m128_f32[0] = rand() % 10 - 5;
		position.m128_f32[2] = rand() % 1 + 30 + pos.m128_f32[2];
	}
	if (spown == true) {
		life = 3;
		spown = false;
	}


	if (life > 0) {
		Active(StopT, positionP);
	}

	Set();

	enemy->Update();
}

void Enemy::Draw()
{
	if (arive == true) {
		enemy->Draw();
	}
}

void Enemy::ImGuiDraw()
{
	float l = life;

	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.7f, 0.7f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.1f, 0.0f, 0.1f, 0.0f));
	ImGui::SetWindowSize(ImVec2(400, 500), ImGuiCond_::ImGuiCond_FirstUseEver);
	ImGui::Begin("Enemy");

	//フラグを手動で切りたい時
	ImGui::Checkbox("arive", &arive);
	//スライダーで動きをいじりたいとき
	ImGui::SliderFloat("life", &l, -100.0f, 100.0f);
	ImGui::SliderFloat("pos", &position.m128_f32[2], -100.0f, 100.0f);
	ImGui::SliderFloat("speed", &speed, -100.0f, 100.0f);
	ImGui::End();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
}

void Enemy::Finalize()
{
	delete model;
}

void Enemy::Active(bool& StopT, XMVECTOR positionP)
{

	float vx = (positionP.m128_f32[0] - position.m128_f32[0]);
	float vz = (positionP.m128_f32[2] - position.m128_f32[2]);
	float v2x = pow(vx, 2);
	float v2z = pow(vz, 2);
	float l = sqrtf(v2x + v2z);
	float v3x = (vx / l) * speed;
	float v3z = (vz / l) * speed;

	position.m128_f32[0] += v3x;
	position.m128_f32[2] += v3z;

	enemy->SetPosition(position);

	if (StopT == true) {
		timer += 0.2f;
		if (timer <= 10) {
			speed = 0;
		}
		else if (timer > 10) {
			StopT = false;
			timer = 0;
		}
	}

	if (StopT == false) {
		if (position.m128_f32[2] <= positionP.m128_f32[2] + 3) {
			speed = 0;
		}
		else {
			speed = 0.05f;
		}
	}

}

XMVECTOR Enemy::GetWorldPos()
{
	XMVECTOR worldPos;

	worldPos.m128_f32[0] = position.m128_f32[0];
	worldPos.m128_f32[1] = position.m128_f32[1];
	worldPos.m128_f32[2] = position.m128_f32[2];
	worldPos.m128_f32[3] = 1;

	return worldPos;
}

