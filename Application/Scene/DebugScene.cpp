#include "DebugScene.h"
#include"ModelManager.h"

DebugScene::DebugScene(SceneManager* sceneManager_)
	:BaseScene(sceneManager_)
{
}

void DebugScene::Initialize(DirectXCommon* dxComon)
{
	camera = new Camera(WinApp::window_width, WinApp::window_height);
	light = Light::Create();
	light->SetLightColor({ 1,1,1 });

	lightGroupe = LightGroup::Create();


	Object3d::SetCamera(camera);
	Object3d::SetLight(light);
	Object3d::SetLightGroup(lightGroupe);


	SphereModel = ObjModel::CreateFromOBJ("skydome");
	Sphere = Object3d::Create();
	Sphere->SetModel(SphereModel);

	GroundModel = ObjModel::CreateFromOBJ("World", true);
	Ground = Object3d::Create(GroundModel);

	sphere = Object3d::Create(ModelManager::GetInstance()->GetModel(13));

	lightGroupe->SetDirLightActive(0, false);
	lightGroupe->SetDirLightActive(1, false);
	lightGroupe->SetDirLightActive(2, false);
	lightGroupe->SetPointLightActive(0, true);
	PointLightsPos.x = 0.0f;
	PointLightsPos.y = 0.0f;
	PointLightsPos.z = 0.0f;

	camera->SetTarget(cameraTarget);
	camera->SetEye(cameraEye);
	camera->RecalculationMatrix();
}

void DebugScene::SetPosSclRot()
{


	Sphere->SetScale(SphereScl);
	Sphere->SetRotation(SphereRot);
	Sphere->SetPosition(SpherePos);

	XMFLOAT3 Rot = sphere->GetRotation();
	Rot.y += 1.0f;

	sphere->SetScale(sphereScl);
	sphere->SetRotation(Rot);
	sphere->SetPosition(spherePos);

	Ground->SetScale(GroundScl);
	Ground->SetPosition(GroundPos);
	Ground->SetRotation(GroundRot);


	camera->SetTarget(cameraTarget);
	camera->SetEye(cameraEye);
	camera->RecalculationMatrix();
	lightGroupe->SetPointLightPos(0, PointLightsPos);
	lightGroupe->SetPointLightColor(0, XMFLOAT3(PoinLightColor));
	lightGroupe->SetPointLightAtten(0, XMFLOAT3(PointLightAtten));
}

void DebugScene::Update()
{
	

	
	if (Input::GetInstance()->PushKey(DIK_W)) { PointLightsPos.z += 1.0f; }
	else if(Input::GetInstance()->PushKey(DIK_S)) { PointLightsPos.z -= 1.0f; }
	if (Input::GetInstance()->PushKey(DIK_D)) { PointLightsPos.x += 1.0f; }
	else if (Input::GetInstance()->PushKey(DIK_A)) { PointLightsPos.x -= 1.0f; }

	light->SetLightDir(lightDir);

	Action::GetInstance()->PlayerMove3d(SpherePos);
	SetPosSclRot();
	light->Update();
	lightGroupe->Update();
	sphere->Update();
	Sphere->Update();
	Ground->Update();
}

void DebugScene::Draw(DirectXCommon* dxCommon)
{
	dxCommon->PreDraw();
	Object3d::PreDraw(dxCommon->GetCmdList());
	sphere->Draw();
	Ground->Draw();
	Sphere->Draw();
	Object3d::PostDraw();
	dxCommon->PostDraw();
}

void DebugScene::Finalize()
{

}
