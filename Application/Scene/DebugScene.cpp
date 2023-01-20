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

	bomodel = ObjModel::CreateFromOBJ("shadow");
	bo = Object3d::Create(bomodel);

	Sprite::LoadTexture(200, L"Resources/mark.png");
	spriteRet.reset(Sprite::SpriteCreate(200, retpos, spCol, anc));

	testEn = Object3d::Create(bomodel);
	testPos = testEn->GetPosition();

	partM = ParticleManager::Create(camera);

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

	XMMATRIX testmat = testEn->GetMatrix();
	worldTest = XMVector3Transform(testPos, testmat);
	testEn->SetPosition(testPos);
	testEn->SetCamera(camera);

	for (int i = 0; i < 10; i++) {
		const float rnd_pos = 0.0f;
		XMFLOAT3 pos;
		pos.x = positionRet.m128_f32[0];
		pos.y = positionRet.m128_f32[1];
		pos.z = positionRet.m128_f32[2];
		const float rnd_vel = 0.3f;
		XMFLOAT3 vel{};
		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

		XMFLOAT3 acc{};

		acc.y = 0.3;
		//if (Input::GetInstance()->PushClick(1)) {
		partM->Add(10, pos, vel, acc, 1.0f, 0.2f, time);
		//}
	}

}

void DebugScene::Update()
{
	
	XMMATRIX mat = bo->GetMatrix();

	XMVECTOR pos = XMVector3TransformNormal(position, mat);
	

	if (Input::GetInstance()->TriggerKey(DIK_A)) {
		testPos.m128_f32[0] += 0.1f;
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


	SetPosSclRot();
	MouthContoroll();
	cam->Update(vel, Eye_rot, camera);
	camera->Update();
	
	bo->Update();
	testEn->Update();
	partM->Update(color);
	//camera->Update();
	//mid->Update();
}

void DebugScene::Draw(DirectXCommon* dxCommon)
{
	dxCommon->PreDraw();
	/*for (int j = 0; j < 3; j++) {
		rob[j]->Draw(dxCommon);
	}*/
	//player->ParticleDraw(dxCommon->GetCmdList());

	ParticleManager::PreDraw(dxCommon->GetCmdList());
	partM->Draw();
	ParticleManager::PostDraw();

	Object3d::PreDraw(dxCommon->GetCmdList());
	bo->Draw();
	testEn->Draw();
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

	positionRet = playerWorldPos;

	//Mouse::GetInstance()->Mousemove(View, Pro, matViewport, retpos, positionRet);


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
