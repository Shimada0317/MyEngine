#include "ClearScene.h"

ClearScene::ClearScene(SceneManager* sceneManager_)
	:BaseScene(sceneManager_)
{
}

void ClearScene::Initialize(DirectXCommon* dxComon)
{
}

void ClearScene::SetPosSclRot()
{
}

void ClearScene::Update()
{
}

void ClearScene::Draw(DirectXCommon* dxCommon)
{
	dxCommon->PreDraw();

	dxCommon->PostDraw();
}

void ClearScene::Finalize()
{
}
