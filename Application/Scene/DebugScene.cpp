#include "DebugScene.h"

DebugScene::DebugScene(SceneManager* sceneManager_)
	:BaseScene(sceneManager_)
{
}

void DebugScene::Initialize(DirectXCommon* dxComon)
{

}

void DebugScene::SetPosSclRot()
{
	

}

void DebugScene::Update()
{

}

void DebugScene::Draw(DirectXCommon* dxCommon)
{
	dxCommon->PreDraw();

	dxCommon->PostDraw();
}

void DebugScene::Finalize()
{

}
