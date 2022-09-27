#include "Robot.h"
#include"Action.h"

void Robot::Initialize()
{
	head = std::make_unique<Head>();
	body = std::make_unique<Body>();
	LArm = std::make_unique<LeftArm>();
	RArm = std::make_unique<RightArm>();
	par = std::make_unique<ObjParticle>();

	part = ParticleManager::Create();

	head->Initialize();
	body->Initialize();
	LArm->Initialize();
	RArm->Initialize();
	par->Initialize();

	for (int i = 0; i < 4; i++) {
		arive[i] = true;
	}

}

void Robot::SetPRS()
{
	for (int i = 0; i < 100; i++) {
		const float rnd_pos = 1.0f;
		XMFLOAT3 pos{};
		pos.x = allPos.m128_f32[0];
		pos.y = allPos.m128_f32[1];
		pos.z = allPos.m128_f32[2];
		//pos.x = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		//pos.y = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		//pos.z = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;

		const float rnd_vel = 0.1f;
		XMFLOAT3 vel{};
		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

		XMFLOAT3 acc{};
		const float rnd_acc = 0.001f;
		acc.y = -(float)rand() / RAND_MAX * rnd_acc;
		
		part->Add(30, pos, vel, acc, 0.5f, 0.0f,time);
		break;
	}

	head->SetPRS(allPos);
	RArm->SetPRS(allPos);
	LArm->SetPRS(allPos);
	body->SetPRS(allPos);
	par->Set(allPos,arive[3]);
}

void Robot::AllUpdate()
{
	head->Update(arive[0], allPos);
	RArm->Update(arive[1], allPos);
	LArm->Update(arive[2], allPos);
	body->Update(arive[3], allPos);
	part->Update();
	if (arive[0] == false && arive[1] == false && arive[2] == false && arive[3] == false) {
		time += 0.1f;
		par->Update();
	}
}

void Robot::Update()
{
	Action::GetInstance()->PlayerMove3d(allPos);

	if (Input::GetInstance()->PushKey(DIK_0)) {
		for (int i = 0; i < 4; i++) {
			arive[i] = true;
			time = 0.0f;
		}
	}

	if (Input::GetInstance()->PushKey(DIK_1)) {
		arive[1] = false;
	}
	if (Input::GetInstance()->PushKey(DIK_2)) {
		arive[2] = false;
	}
	if (Input::GetInstance()->PushKey(DIK_3)) {
		arive[3] = false;
	}

	if (arive[3] == false || arive[0] == false) {
		for (int i = 0; i < 4; i++) {
			arive[i] = false;
		}
	}

	attackT += 0.5f;

	RArm->Attack(attackT);
	LArm->Attack(attackT);

	if (attackT >= 10.0f) {
		attackT = 0.0f;
	}
	
	SetPRS();
	AllUpdate();
}

void Robot::Draw(DirectXCommon* dxCommon)
{
	head->Draw(arive[0]);
	RArm->Draw(arive[1]);
	LArm->Draw(arive[2]);
	body->Draw(arive[3]);
	if (arive[0] == false && arive[1] == false && arive[2] == false && arive[3] == false) {
		//par->Draw();
		ParticleManager::PreDraw(dxCommon->GetCmdList());
		part->Draw();
		ParticleManager::PostDraw();
	}
}

void Robot::Finalize()
{
	head->Finalize();
	body->Finalize();
	LArm->Finalize();
	RArm->Finalize();
}
