#include "Enemy.h"
#include<cassert>
#include"imgui/imgui.h"

void Enemy::Initalize()
{
	input = Input::GetInstance();
	debugtext = DebugText::GetInstance();
	model = ObjModel::CreateFromOBJ("enemy");
	enemy->CreateGraphicsPipeline(L"Resources/shaders/BasicVS.hlsl", L"Resources/shaders/BasicPS.hlsl");
	enemy= Object3d::Create();
	enemy->SetModel(model);

}

void Enemy::Set()
{
	enemy->SetPosition(position);
	enemy->SetRotation(rotation);
	enemy->SetScale(scale);

}

void Enemy::Update(XMFLOAT3 pos)
{

	if (life <= 0) {
		arive = false;
	}

	if (arive == false) {
		position.x = rand() % 10 - 5;
		position.z = rand() % 1 + 30+pos.z;
		responetime += 0.2f;
	}

	if (responetime >= 10.0f) {
		life = 3;
		responetime = 0.0f;
		arive = true;
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
	ImGui::SliderFloat("pos", &position.z, -100.0f, 100.0f);
	ImGui::SliderFloat("speed", &speed, -100.0f, 100.0f);
	ImGui::End();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
}

void Enemy::Finalize()
{
	delete enemy;
	delete model;
}

void Enemy::Active(bool& StopT,int action,XMFLOAT3 positionP)
{
	if (action == 0) {
		position.z -= speed;
	}

	if (action == 1) {
		float vx = (positionP.x - position.x);
		float vz = (positionP.z - position.z);
		float v2x = pow(vx, 2);
		float v2z = pow(vz, 2);
		float l = sqrtf(v2x + v2z);
		float v3x= (vx / l) * speed;
		float v3z = (vz / l) * speed;

		position.x += v3x;
		position.z += v3z;
	}
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
		if (position.z <= positionP.z+3) {
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

	worldPos.m128_f32[0] = position.x;
	worldPos.m128_f32[1] = position.y;
	worldPos.m128_f32[2] = position.z;
	worldPos.m128_f32[3] = 1;

	return worldPos;
}

