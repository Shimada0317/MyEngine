#include "DebugScene.h"

DebugScene::DebugScene(SceneManager* sceneManager_)
	:BaseScene(sceneManager_)
{
}

void DebugScene::Initialize(DirectXCommon* dxComon)
{
	Texture::LoadTexture(199, L"Resources/11257266-000011-rGvfiJH3.dds");
	tex = Texture::Create(199, position, size, color);
	tex->CreateNormalTexture();
	tex->Update();
}

void DebugScene::SetPosSclRot()
{
	
	tex->SetPosition({ position });
	tex->SetEye({ cameraEye });
	tex->SetTarget({ cameraTarget });
	tex->CameraMoveVector({ position });
}

void DebugScene::Update()
{
	SetPosSclRot();
	tex->Update();
}

void DebugScene::Draw(DirectXCommon* dxCommon)
{
	dxCommon->PreDraw();
	Texture::PreDraw(dxCommon->GetCmdList());
	tex->Draw();
	Texture::PostDraw();
	dxCommon->PostDraw();
}

void DebugScene::Finalize()
{

}
