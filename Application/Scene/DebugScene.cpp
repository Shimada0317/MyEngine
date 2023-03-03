#include "DebugScene.h"
#include"ModelManager.h"

DebugScene::DebugScene(SceneManager* sceneManager_)
	:BaseScene(sceneManager_)
{
}

void DebugScene::Initialize(DirectXCommon* dxComon)
{
	camera = new Camera(WinApp::window_width, WinApp::window_height);
	//light = Light::Create();
	//light->SetLightColor({ 1,1,1 });

	lightGroupe = LightGroup::Create();


	Object3d::SetCamera(camera);
	//Object3d::SetLight(light);
	Object3d::SetLightGroup(lightGroupe);


	SphereModel = ObjModel::CreateFromOBJ("skydome");
	Sphere = Object3d::Create();
	Sphere->SetModel(SphereModel);

	GroundModel = ObjModel::CreateFromOBJ("World", true);
	Ground = Object3d::Create(GroundModel);

	sphere = Object3d::Create(ModelManager::GetInstance()->GetModel(13));

	wall = Object3d::Create(ModelManager::GetInstance()->GetModel(4));

	/*lightGroupe->SetDirLightActive(0, false);
	lightGroupe->SetDirLightActive(1, false);
	lightGroupe->SetDirLightActive(2, false);
	lightGroupe->SetPointLightActive(0, true);
	PointLightsPos.x = 0.5f;
	PointLightsPos.y = 1.0f;
	PointLightsPos.z = 0.0f;*/

	lightGroupe->SetPointLightActive(0, false);
	lightGroupe->SetPointLightActive(1, false);
	lightGroupe->SetPointLightActive(2, false);
	lightGroupe->SetSpotLightActive(0, true);
	lightGroupe->SetSpotLightActive(1, true);
	lightGroupe->SetSpotLightActive(2, true);

	camera->SetTarget(cameraTarget);
	camera->SetEye(cameraEye);
	camera->RecalculationMatrix();
}

void DebugScene::SetPosSclRot()
{


	Sphere->SetScale(SphereScl);
	Sphere->SetRotation(SphereRot);
	Sphere->SetPosition(SpherePos);

	wall->SetScale(SphereScl);
	wall->SetRotation(SphereRot);
	wall->SetPosition(spherePos);

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

	/*lightGroupe->SetPointLightPos(0, PointLightsPos);
	lightGroupe->SetPointLightColor(0, XMFLOAT3(PoinLightColor));
	lightGroupe->SetPointLightAtten(0, XMFLOAT3(PointLightAtten));*/

	lightGroupe->SetSpotLightDir(0, XMVECTOR({ SpotLightDir.x, SpotLightDir.y, SpotLightDir.z }));
	lightGroupe->SetSpotLightPos(0, SpotLightPos);
	lightGroupe->SetSpotLightColor(0, SpotLightColor);
	lightGroupe->SetSpotLightAtten(0, SpotLightAtten);
	lightGroupe->SetSpotLightFactorAngle(0, SpotLightFactorAngle);

	lightGroupe->SetSpotLightDir(1, XMVECTOR({ SpotLightDir2.x, SpotLightDir2.y, SpotLightDir2.z }));
	lightGroupe->SetSpotLightPos(1, SpotLightPos2);
	lightGroupe->SetSpotLightColor(1, SpotLightColor2);
	lightGroupe->SetSpotLightAtten(1, SpotLightAtten2);
	lightGroupe->SetSpotLightFactorAngle(1, SpotLightFactorAngle2);


}

void DebugScene::Update()
{
	

	
	if (Input::GetInstance()->PushKey(DIK_W)) { SpotLightPos.z += 1.0f; }
	else if(Input::GetInstance()->PushKey(DIK_S)) { SpotLightPos.z -= 1.0f; }
	if (Input::GetInstance()->PushKey(DIK_D)) { SpotLightPos.x += 1.0f; }
	else if (Input::GetInstance()->PushKey(DIK_A)) { SpotLightPos.x -= 1.0f; }


	if (Input::GetInstance()->PushKey(DIK_UP)) { SpotLightDir.y += 1.0f; }
	else if (Input::GetInstance()->PushKey(DIK_DOWN)) { SpotLightDir.y -= 1.0f; }
	if (Input::GetInstance()->PushKey(DIK_RIGHT)) { SpotLightDir.x += 0.01f; }
	else if (Input::GetInstance()->PushKey(DIK_LEFT)) { SpotLightDir.x -= 0.01f; }

//	light->SetLightDir(lightDir);

	//Action::GetInstance()->PlayerMove3d(SpherePos);
	SetPosSclRot();
	//light->Update();
	lightGroupe->Update();
	sphere->Update();
	Sphere->Update();
	wall->Update();
	Ground->Update();
}

void DebugScene::Draw(DirectXCommon* dxCommon)
{
	dxCommon->PreDraw();
	Object3d::PreDraw(dxCommon->GetCmdList());
	wall->Draw();
	sphere->Draw();
	Ground->Draw();
	Sphere->Draw();
	Object3d::PostDraw();
	dxCommon->PostDraw();
}

void DebugScene::Finalize()
{

}
