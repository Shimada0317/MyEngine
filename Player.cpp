#include "Player.h"
#include<cassert>
#include "imgui/imgui.h"

void Player::Initalize()
{

	input = Input::GetInstance();
	debugtext = DebugText::GetInstance();
	model = ObjModel::LoadFromObJ("block0");
	player = Object3d::Create();
	player->SetModel(model);

	position = player->GetPosition();

	texmodel = TextureModel::loadFromTex(L"Resources/mark.png");
	tex = Texture::Create();
	tex->SetModel(texmodel);
	tex->Update();
};

void Player::Set()
{


	
	//player->SetPosition(position);
	//player->SetEye({ cameraEye });
	//player->SetTarget({ cameraTarget });
	//player->CameraMoveVector({ position.x+3,position.y,position.z });
	//if (stopF == true) {
	//	position.x = camerapos.x;
	//	position.y = camerapos.y;
	//	position.z = camerapos.z;
	//}
	//else if (stopF == false) {
	//	player->SetPosition(position);
	//}

	
	tex->SetScale({ 0.3f,0.3f,0.3f });
	tex->SetPosition({ position });
	tex->SetEye({ cameraEye });
	tex->SetTarget({ cameraTarget });
	if (stopF == true) {
		tex->CameraMoveVector({ position });
	}
}

void Player::Update()
{
	Action::GetInstance()->PlayerMove2d(position, 0.1f);
	if (Input::GetInstance()->PushKey(DIK_2)) {
		stopF = true;
	}
	else if(Input::GetInstance()->PushKey(DIK_3)) {
		stopF = false;
	}


	Set();
	tex->Update();
	player->Update();
}

void Player::Draw(ID3D12GraphicsCommandList* cmdList)
{
	Texture::PreDraw(cmdList);
	tex->Draw();
	Texture::PostDraw();

	//player->Draw();
}

void Player::ObjDraw()
{
	player->Draw();
}

void Player::ImGuiDraw()
{
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.7f, 0.7f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.0f, 0.3f, 0.1f, 0.0f));
	ImGui::SetWindowSize(ImVec2(400, 500), ImGuiCond_::ImGuiCond_FirstUseEver);
	ImGui::Begin("Player");
	bool blnCk = false;
	//フラグを手動で切りたい時
	//ImGui::Checkbox("ramieruposition", &blnCk);
	//スライダーで動きをいじりたいとき
	ImGui::SliderFloat("player.x", &position.x, -100.0f, 100.0f);
	ImGui::SliderFloat("player.y", &position.y, -100.0f, 100.0f);
	ImGui::SliderFloat("player.z", &position.z, -100.0f, 100.0f);

	ImGui::SliderFloat("Eye.x", &cameraEye.x, -100.0f, 100.0f);
	ImGui::SliderFloat("Eye.y", &cameraEye.y, -100.0f, 100.0f);
	ImGui::SliderFloat("Eye.z", &cameraEye.z, -100.0f, 100.0f);

	ImGui::SliderFloat("Target.x", &cameraTarget.x, -100.0f, 100.0f);
	ImGui::SliderFloat("Target.y", &cameraTarget.y, -100.0f, 100.0f);
	ImGui::SliderFloat("Target.z", &cameraTarget.z, -100.0f, 100.0f);

	ImGui::End();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
}

void Player::Finalize()
{
	delete tex;
	delete texmodel;
	delete player;
	delete model;
}
