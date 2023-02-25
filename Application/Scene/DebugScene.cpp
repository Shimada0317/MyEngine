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
	light->SetLightColor({ 0,1,1 });
	Object3d::SetCamera(camera);
	Object3d::SetLight(light);

	SphereModel = ObjModel::CreateFromOBJ("skydome");
	Sphere = Object3d::Create();
	Sphere->SetModel(SphereModel);

	GroundModel = ObjModel::CreateFromOBJ("World", true);
	Ground = Object3d::Create(GroundModel);

	sphere = Object3d::Create(ModelManager::GetInstance()->GetModel(13));

	/*lightGroupe->SetDirLightActive(0, false);
	lightGroupe->SetDirLightActive(1, false);
	lightGroupe->SetDirLightActive(2, false);
	lightGroupe->SetPointLightActive(0, true);*/
	PointLightsPos[0] = 0.5f;
	PointLightsPos[1] = 1.0f;
	PointLightsPos[2] = 0.0f;

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
	/*lightGroupe->SetPointLightPos(0, XMFLOAT3(PointLightsPos));
	lightGroupe->SetPointLightColor(0, XMFLOAT3(PoinLightColor));
	lightGroupe->SetPointLightAtten(0, XMFLOAT3(PointLightAtten));*/
}

void DebugScene::Update()
{
	

	
	if (Input::GetInstance()->PushKey(DIK_W)) { lightDir.m128_f32[1] += 1.0f; }
	else if(Input::GetInstance()->PushKey(DIK_S)) { lightDir.m128_f32[1] -= 1.0f; }
	if (Input::GetInstance()->PushKey(DIK_D)) { lightDir.m128_f32[0] += 1.0f; }
	else if (Input::GetInstance()->PushKey(DIK_A)) { lightDir.m128_f32[0] -= 1.0f; }

	light->SetLightDir(lightDir);

	Action::GetInstance()->PlayerMove3d(SpherePos);
	SetPosSclRot();
	light->Update();
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
