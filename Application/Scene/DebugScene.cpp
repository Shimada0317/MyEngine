#include "DebugScene.h"

DebugScene::DebugScene(SceneManager* sceneManager_)
	:BaseScene(sceneManager_)
{
}

void DebugScene::Initialize(DirectXCommon* dxComon)
{
	Texture::LoadTexture(199, L"Resources/Saigo_Takamori.dds");
	tex = Texture::Create(199, position, size, color);
	tex->CreateNormalTexture();
	tex->Update();

	SphereModel = ObjModel::CreateFromOBJ("skydome");
	Sphere = Object3d::Create();
	Sphere->SetModel(SphereModel);

	GroundModel = ObjModel::CreateFromOBJ("World", true);
	Ground = Object3d::Create(GroundModel);

	/*lightGroupe->SetDirLightActive(0, false);
	lightGroupe->SetDirLightActive(1, false);
	lightGroupe->SetDirLightActive(2, false);
	lightGroupe->SetPointLightActive(0, true);*/
	PointLightsPos[0] = 0.5f;
	PointLightsPos[1] = 1.0f;
	PointLightsPos[2] = 0.0f;
}

void DebugScene::SetPosSclRot()
{

	XMMATRIX matWorld = Sphere->GetMatrix();

	SphereWPos = XMVector3TransformNormal(SpherePos, matWorld);

	Sphere->SetScale(SphereScl);
	Sphere->SetPosition(SphereWPos);
	Sphere->SetRotation(SphereRot);

	Ground->SetScale(GroundScl);
	Ground->SetPosition(GroundPos);
	Ground->SetRotation(GroundRot);
	
	tex->SetPosition({ position });
	tex->SetEye({ cameraEye });
	tex->SetTarget({ cameraTarget });
	tex->CameraMoveVector({ position });

	/*lightGroupe->SetPointLightPos(0, XMFLOAT3(PointLightsPos));
	lightGroupe->SetPointLightColor(0, XMFLOAT3(PoinLightColor));
	lightGroupe->SetPointLightAtten(0, XMFLOAT3(PointLightAtten));*/
}

void DebugScene::Update()
{
	if (Input::GetInstance()->TriggerKey(DIK_A)) {
		SphereRot.y += 10;
	}

	Action::GetInstance()->PlayerMove3d(SpherePos);
	SetPosSclRot();
	tex->Update();
	Sphere->Update();
	Ground->Update();
}

void DebugScene::Draw(DirectXCommon* dxCommon)
{
	dxCommon->PreDraw();
	Texture::PreDraw(dxCommon->GetCmdList());
	tex->Draw();
	Texture::PostDraw();
	Object3d::PreDraw(dxCommon->GetCmdList());
	Sphere->Draw();
	Ground->Draw();
	Object3d::PostDraw();
	dxCommon->PostDraw();
}

void DebugScene::Finalize()
{

}
