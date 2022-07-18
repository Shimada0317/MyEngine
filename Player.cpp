#include "Player.h"
#include<cassert>
#include "imgui/imgui.h"
#include"imgui/imconfig.h"

void Player::Initalize()
{
	Sprite::LoadTexture(3, L"Resources/mark.png");
	input = Input::GetInstance();
	debugtext = DebugText::GetInstance();



	model = ObjModel::CreateFromOBJ("drugon");
	player->CreateGraphicsPipeline(L"Resources/shaders/toonVS.hlsl", L"Resources/shaders/toonPS.hlsl");
	player = Object3d::Create();
	player->SetModel(model);

	Texture::LoadTexture(100, L"Resources/mark.png");
	tex = Texture::Create(100,position,size,color);
	tex->CreateNormalTexture();
	tex->Update();
};

void Player::Set()
{
	player->SetRotation(rotation);
	player->SetPosition(position);
	player->SetScale(scael);

}

void Player::Update()
{
	Action::GetInstance()->PlayerMove2d(position, 0.1f);
	Action::GetInstance()->PlayerMove2d(rotation, 0.1f);

	Set();
	tex->Update();
	player->Update();
}

void Player::Draw(ID3D12GraphicsCommandList* cmdList)
{
	/*Texture::PreDraw(cmdList);
	tex->Draw();
	Texture::PostDraw();*/

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
	ImGui::Checkbox("shader", &Changeshader);
	//スライダーで動きをいじりたいとき

	if (ImGui::TreeNode("position")) {
		ImGui::SliderFloat("player.x", &position.x, -100.0f, 100.0f);
		ImGui::SliderFloat("player.y", &position.y, -100.0f, 100.0f);
		ImGui::SliderFloat("player.z", &position.z, -100.0f, 100.0f);
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("rotation")) {
		ImGui::SliderFloat("rot.x", &rotation.x, -100.0f, 100.0f);
		ImGui::SliderFloat("rot.y", &rotation.y, -100.0f, 100.0f);
		ImGui::SliderFloat("rot.z", &rotation.z, -100.0f, 100.0f);
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("cameraEye")) {
		ImGui::SliderFloat("Eye.x", &cameraEye.x, -100.0f, 100.0f);
		ImGui::SliderFloat("Eye.y", &cameraEye.y, -100.0f, 100.0f);
		ImGui::SliderFloat("Eye.z", &cameraEye.z, -100.0f, 100.0f);
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("cameraTarget")) {
		ImGui::SliderFloat("Target.x", &cameraTarget.x, -100.0f, 100.0f);
		ImGui::SliderFloat("Target.y", &cameraTarget.y, -100.0f, 100.0f);
		ImGui::SliderFloat("Target.z", &cameraTarget.z, -100.0f, 100.0f);
		ImGui::TreePop();
	}
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
	delete playernext;
	delete modelnext;
}
