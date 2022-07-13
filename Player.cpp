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
	//Eye_pos = { position.x + cameraEye.x,position.y + cameraEye.y,position.z + cameraEye.z };
	//Target_pos = { position.x + cameraTarget.x,position.y + cameraTarget.y,position.z + cameraTarget.z };

	camerapos = { position };
	player->SetPosition(position);
	player->SetEye({ cameraEye });
	player->SetTarget({ cameraTarget });
	if (stopF == true) {
	player->CameraMoveVector({ camerapos });
	player->SetPosition(camerapos);
	position = { camerapos };
	}
	//tex->SetScale({ 0.3f,0.3f,0.3f });
	/*tex->SetPosition({ position });
	tex->SetEye({ cameraEye });
	tex->SetTarget({ cameraTarget });
	tex->CameraMoveVector({ position });*/


	//tex->SetEye({ cameraEye });
	//tex->SetTarget({ cameraTarget });
	//tex->CameraMoveVector({ position });
}

void Player::Update()
{

	Action::GetInstance()->PlayerMove2d(position, 0.03f);
	

	if (Input::GetInstance()->PushKey(DIK_2)) {
		stopF = true;
	}
	else if(Input::GetInstance()->PushKey(DIK_3)) {
		stopF = false;
	}


	

	//Action::GetInstance()->PlayerMove2d(cameraTarget, 0.5f);
	Set();
	//tex->Update();
	player->Update();
}

void Player::Draw(ID3D12GraphicsCommandList* cmdList)
{
	
	/*Texture::PreDraw(cmdList);
	tex->Draw();
	Texture::PostDraw();*/
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
