#include "Player.h"
#include<cassert>
#include "imgui/imgui.h"
#include"imgui/imconfig.h"
#include"WinApp.h"

void Player::Initalize()
{
	Sprite::LoadTexture(3, L"Resources/mark.png");
	input = Input::GetInstance();
	debugtext = DebugText::GetInstance();

	camera = new DebugCamera(WinApp::window_width, WinApp::window_height);

	player->SetCamera(camera);

	model = ObjModel::CreateFromOBJ("mark");
	player->CreateGraphicsPipeline(L"Resources/shaders/BasicVS.hlsl", L"Resources/shaders/BasicPS.hlsl");
	player = Object3d::Create();
	player->SetModel(model);

	Texture::LoadTexture(100, L"Resources/mark.png");
	tex = Texture::Create(100,position,size,color);
	tex->CreateNormalTexture();
	tex->Update();
	camera->SetTarget({ 0.0f,0.2f,0.0f });
	camera->SetEye({ 1,1,1 });
	camera->SetDistance(5.0f);

	bul = new Bullet();
	bul->Initialize();

};

void Player::Set()
{
	player->SetRotation(rotation);
	player->SetPosition(position);
	player->SetScale(scael);
	
	camera->SetEye(position);
	camera->SetTarget(cameraTarget);

	
}

void Player::Update()
{
	Action::GetInstance()->PlayerMove2d(position, moveSpeed);
	Action::GetInstance()->Gunshot(2, shot);
	

	//Action::GetInstance()->PlayerMove2d(rotation, 0.1f);
	if (position.y <= 0.5f) {
		position.y = 0.5f;
	}
	else if (position.y >= 6.2f) {
		position.y = 6.2f;
	}

	if (position.x >= 9.8f) {
		position.x = 9.8f;
	}
	else if (position.x <= -9.8f) {
		position.x = -9.8f;
	}

	//if(position.x)
	bul->bun(position);
	Set();
	tex->Update();
	bul->Update();
	player->Update();
	camera->Update();
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
	bul->Draw();
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

	bul->ImgUiDraw();
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
