#include "Player.h"
#include<cassert>
#include "imgui/imgui.h"
#include"imgui/imconfig.h"
#include"WinApp.h"

void Player::Initalize()
{
	Sprite::LoadTexture(3, L"Resources/mark.png");
	reticle = Sprite::SpriteCreate(3, { 1.0f,1.0f });

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


};

void Player::Set()
{
	player->SetRotation(rotation);
	player->SetPosition(position);
	player->SetScale(scale);
	
	camera->SetEye(cameraEye);
	camera->SetTarget(cameraTarget);
	camera->SetUp(up);
	
}

void Player::Update()
{
	MouthContoroll();

	//if(position.x)
	
	Set();
	tex->Update();
	player->Update();
	camera->Update();
}

void Player::Draw(ID3D12GraphicsCommandList* cmdList)
{
	/*Texture::PreDraw(cmdList);
	tex->Draw();
	Texture::PostDraw();*/

	//player->Draw();
	reticle->PreDraw(cmdList);
	reticle->Draw();
	reticle->PostDraw();
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

void Player::MouthContoroll()
{

	GetCursorPos(&pos);

	ScreenToClient(FindWindowA("DirectXGame",nullptr), &pos);

	position.x = pos.x;
	position.y = pos.y;

	//Input::MousePos mouseMove = Input::GetInstance()->GetMouseMove();
	//position.x = mouseMove.lX;
	//position.y = mouseMove.lY;

	//XMMATRIX matProjection = XMMatrixPerspectiveFovLH(
	//	XMConvertToRadians(60.0f),
	//	(float)WinApp::window_width / WinApp::window_height,
	//	0.1f, 1000.0f
	//);

	//const XMMATRIX& matViewProjection = camera->GetViewProjectionMatrix();

	//XMMATRIX matVPV = XMMatrixLookAtLH(XMLoadFloat3(&Eye_pos), XMLoadFloat3(&cameraTarget), XMLoadFloat3(&up))*matProjection;

	//XMMATRIX matIverserVPV = XMMatrixInverse(nullptr,matVPV);

	//XMVECTOR posNear = XMVECTOR(0, 0, 0);
	//XMVECTOR posFar

	//posNear=
	//posFar
}
