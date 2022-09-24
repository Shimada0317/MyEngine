#include "Robot.h"
#include"Action.h"

void Robot::Initialize()
{
	head = std::make_unique<Head>();
	body = std::make_unique<Body>();
	LArm = std::make_unique<LeftArm>();
	RArm = std::make_unique<RightArm>();
	par = std::make_unique<ObjParticle>();

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
	if (arive[0] == false && arive[1] == false && arive[2] == false && arive[3] == false) {
		par->Update();
	}
	
}

void Robot::Update()
{
	Action::GetInstance()->PlayerMove3d(allPos);
	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		
		
	}

	if (Input::GetInstance()->PushKey(DIK_0)) {
		for (int i = 0; i < 4; i++) {
			arive[i] = true;
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

void Robot::Draw()
{
	head->Draw(arive[0]);
	RArm->Draw(arive[1]);
	LArm->Draw(arive[2]);
	body->Draw(arive[3]);
	if (arive[0] == false && arive[1] == false && arive[2] == false && arive[3] == false) {
		par->Draw();
	}
}

void Robot::Finalize()
{
	head->Finalize();
	body->Finalize();
	LArm->Finalize();
	RArm->Finalize();
}
