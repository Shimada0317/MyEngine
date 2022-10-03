#include "Robot.h"
#include"Action.h"

void Robot::Initialize()
{
	head = std::make_unique<Head>();
	body = std::make_unique<Body>();
	LArm = std::make_unique<LeftArm>();
	RArm = std::make_unique<RightArm>();

	head->Initialize();
	body->Initialize();
	LArm->Initialize();
	RArm->Initialize();

	for (int i = 0; i < 4; i++) {
		arive[i] = true;
	}

}

void Robot::SetPRS(Bullet* bull)
{


	head->SetPRS(allPos,bull);
	RArm->SetPRS(allPos);
	LArm->SetPRS(allPos,bull);
	body->SetPRS(allPos,bull);
}

void Robot::AllUpdate(Bullet* bull)
{
	head->Update(arive[0], allPos,bull);
	RArm->Update(arive[1], allPos);
	LArm->Update(arive[2], allPos,bull);
	body->Update(arive[3], allPos,bull);
	if (arive[0] == false && arive[1] == false && arive[2] == false && arive[3] == false) {
		time += 0.1f;
	}
}

void Robot::Update(Bullet* bull)
{
	//Action::GetInstance()->PlayerMove3d(allPos);

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
	
	SetPRS(bull);
	AllUpdate(bull);
}

void Robot::Draw(DirectXCommon* dxCommon)
{
	Object3d::PreDraw(dxCommon->GetCmdList());
	head->Draw(arive[0]);
	RArm->Draw(arive[1]);
	LArm->Draw(arive[2]);
	body->Draw(arive[3]);
	Object3d::PostDraw();
}

void Robot::ParticleDraw(DirectXCommon* dxCommon)
{

}

void Robot::Finalize()
{
	head->Finalize();
	body->Finalize();
	LArm->Finalize();
	RArm->Finalize();
}
