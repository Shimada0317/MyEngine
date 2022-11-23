#include "DebugScene.h"

DebugScene::DebugScene(SceneManager* sceneManager_)
	:BaseScene(sceneManager_)
{
}

void DebugScene::Initialize(DirectXCommon* dxComon)
{
	light = Light::Create();
	Object3d::SetLight(light);

	camera = new Camera(WinApp::window_width, WinApp::window_height);
	Object3d::SetCamera(camera);

	bomodel = ObjModel::CreateFromOBJ("mark");
	bo = Object3d::Create(bomodel);

	Sprite::LoadTexture(200, L"Resources/mark.png");
	spriteRet.reset(Sprite::SpriteCreate(200, retpos, spCol, anc));

	cam = new RailCamera();
	cam->Initialize(position, rotation);

	bo->SetParent(camera);
}

void DebugScene::SetPosSclRot()
{
	const float BulletSpeed = 15.0f;
	velocity = { 0, 0, BulletSpeed };
	velocity = XMVector3TransformNormal(velocity, mat);
	playerWorldPos = { 0.0f,0.0f,0.0f };
	playerWorldPos = XMVector3Transform(playerWorldPos, mat);

	bo->SetPosition(position);
	bo->SetRotation(rotation);
	bo->SetParent(camera);
}

void DebugScene::Updata()
{
	SetPosSclRot();
	XMMATRIX mat = bo->GetMatrix();
	
	XMVECTOR pos = XMVector3TransformNormal(position, mat);

	Action::GetInstance()->PlayerMove3d(pos);
	if (Input::GetInstance()->PushKey(DIK_A)) {
		rotation.y += 1;
	}
	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		jump = true;
	
	}
	if (jump == true) {
		position.m128_f32[1] = position.m128_f32[1] + addgrav;
		addgrav -= 0.02f;
	}

	if (Input::GetInstance()->PushKey(DIK_A)) {
		kBulletSpeed = 0.5f;
		vel = { 0, 0, kBulletSpeed };
		vel = XMVector3TransformNormal(vel, mat);
	}
	else {
		kBulletSpeed = 0.0f;
		vel = { 0, 0, kBulletSpeed };
		vel = XMVector3TransformNormal(vel, mat);
	}

	//for (int i = 0; i < 9; i++) {
	//	for (int j = 0; j < 3; j++) {
	//		rob[j]->Update(bull[i]);
	//		//break;
	//	}
	//	//break;
	//}
	//
	

	MouthContoroll();
	cam->Updata(vel, Eye_rot, camera);
	camera->Updata();
	bo->Updata();
	//camera->Update();
	//mid->Updata();
}

void DebugScene::Draw(DirectXCommon* dxCommon)
{
	dxCommon->PreDraw();
	/*for (int j = 0; j < 3; j++) {
		rob[j]->Draw(dxCommon);
	}*/
	//player->ParticleDraw(dxCommon->GetCmdList());
	Object3d::PreDraw(dxCommon->GetCmdList());
	bo->Draw();

	Object3d::PostDraw();
	//mid->Draw(dxCommon);
	dxCommon->PostDraw();
}

void DebugScene::ChangeViewPort(XMMATRIX& matViewPort)
{
	matViewPort.r[0].m128_f32[0] = WinApp::window_width / 2;
	matViewPort.r[0].m128_f32[1] = 0;
	matViewPort.r[0].m128_f32[2] = 0;
	matViewPort.r[0].m128_f32[3] = 0;

	matViewPort.r[1].m128_f32[0] = 0;
	matViewPort.r[1].m128_f32[1] = -(WinApp::window_height / 2);
	matViewPort.r[1].m128_f32[2] = 0;
	matViewPort.r[1].m128_f32[3] = 0;

	matViewPort.r[2].m128_f32[0] = 0;
	matViewPort.r[2].m128_f32[1] = 0;
	matViewPort.r[2].m128_f32[2] = 1;
	matViewPort.r[2].m128_f32[3] = 0;

	matViewPort.r[3].m128_f32[0] = WinApp::window_width / 2 + offset.m128_f32[0];
	matViewPort.r[3].m128_f32[1] = WinApp::window_height / 2 + offset.m128_f32[1];
	matViewPort.r[3].m128_f32[2] = 0;
	matViewPort.r[3].m128_f32[3] = 1;
}

void DebugScene::MouthContoroll()
{
	Mouse::GetInstance()->MouseMoveSprite(retpos);

	spriteRet->SetPosition(retpos);

	XMMATRIX matViewport;

	ChangeViewPort(matViewport);

	XMMATRIX View = camera->GetViewMatrix();
	XMMATRIX Pro = camera->GetProjectionMatrix();

	XMVECTOR positionRet = playerWorldPos;

	Mouse::GetInstance()->Mousemove(View, Pro, matViewport, retpos, positionRet);


	bo->SetPosition(positionRet);
}

void DebugScene::Finalize()
{
	//for (int j = 0; j < 3; j++) {
	//	rob[j]->Finalize();
	//}
	//player->Finalize();
	///mid->Fainalize();
}
