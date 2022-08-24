#include "Player.h"
#include<cassert>
#include "imgui/imgui.h"
#include"imgui/imconfig.h"
#include"WinApp.h"

void Player::Initalize()
{
	camera = new DebugCamera(WinApp::window_width, WinApp::window_height);
	Object3d::SetCamera(camera);

	Sprite::LoadTexture(3, L"Resources/mark.png");
	reticle = Sprite::SpriteCreate(3, { 1.0f,1.0f });

	playerModel = ObjModel::CreateFromOBJ("mark");
	player->CreateGraphicsPipeline();
	player.reset(Object3d::Create());
	player->SetModel(playerModel);

	input = Input::GetInstance();
	debugtext = DebugText::GetInstance();

};

void Player::Set()
{
	player->SetPosition({ position });
	player->SetRotation({ rotation });
	player->SetScale({ scale });

	camera->SetEye({ Eye_pos });
	camera->SetTarget({Target_pos.x,Target_pos.y,position.z});
	camera->SetDistance(5);
}

void Player::Update()
{
	if (position.x >= 4) {
		Target_pos.x += 0.1f;
		if (Target_pos.x >= 2.0f) {
			Target_pos.x = 2.0f;
		}
	}
	else if(position.x<=3.9f&&position.x > 0) {
		Target_pos.x -= 0.1f;
		if (Target_pos.x <= 0.0f) {
			Target_pos.x = 0.0f;
		}
	}

	if (position.x <= -4) {
		Target_pos.x -= 0.1f;
		if (Target_pos.x <= -2.0f) {
			Target_pos.x = -2.0f;
		}
	}
	else if(position.x >= -3.9f && position.x < 0)
	{
		Target_pos.x += 0.1f;
		if (Target_pos.x >= 0.0f) {
			Target_pos.x = 0.0f;
		}
	}

	//MouthContoroll();
	Set();
	player->Update();
	camera->Update();
}

void Player::Draw(ID3D12GraphicsCommandList* cmdList)
{
	
	
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
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.1f, 0.0f, 0.1f, 0.0f));
	ImGui::SetWindowSize(ImVec2(400, 500), ImGuiCond_::ImGuiCond_FirstUseEver);
	ImGui::Begin("Plyer");

	if (ImGui::TreeNode("position")) {
		ImGui::SliderFloat("position.x", &position.x, -100.0f, 100.0f);
		ImGui::SliderFloat("position.y", &position.y, -100.0f, 100.0f);
		ImGui::SliderFloat("position.z", &position.z, -100.0f, 100.0f);
		ImGui::SliderFloat("target.x", &Target_pos.x, -100.0f, 100.0f);
		ImGui::SliderFloat("target.y", &Target_pos.y, -100.0f, 100.0f);
		ImGui::TreePop();
	}

	ImGui::End();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
}

void Player::Finalize()
{
	delete camera;
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
	XMMATRIX matViewport = { WinApp::window_width / 2,0,0,0  ,0,-WinApp::window_height / 2,0,0    ,0,0,1,0   ,WinApp::window_width/2+OffsetX,WinApp::window_height/2+OffsetY,0,1 };
	//ビュー、プロジェクション、ビューポート3つの行列の乗算
	XMMATRIX matVPV = XMMatrixLookAtLH(XMLoadFloat3(&Eye_pos), XMLoadFloat3(&cameraTarget), XMLoadFloat3(&up))*matProjection*matViewport;

	XMMATRIX matIverserVPV = XMMatrixInverse(nullptr,matVPV);


	XMVECTOR posNear = { pos.x, pos.y, 0,1 };
	XMVECTOR posFar = { pos.x,pos.y,1,1 };

	posNear = XMVector3TransformCoord( posNear, matIverserVPV );
	posFar = XMVector3TransformCoord(posFar, matIverserVPV);

	XMVECTOR mouseDirection = posNear + posFar;
	mouseDirection=XMVector3Normalize(mouseDirection);

	const float kDistanceTestObject = 10;

	//pos=posNear+mouseDirection
}

XMVECTOR Player::GetWorldPosition()
{
	XMVECTOR worldPos;

	worldPos.m128_f32[0] = position.x;
	worldPos.m128_f32[1] = position.y;
	worldPos.m128_f32[2] = position.z;
	worldPos.m128_f32[3] = 1;

	return worldPos;
}
