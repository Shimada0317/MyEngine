#include "Robot.h"

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

}

void Robot::SetPRS()
{
	head->SetPRS(allPos);
	RArm->SetPRS(allPos);
	LArm->SetPRS(allPos);
	body->SetPRS(allPos);
}

void Robot::AllUpdate()
{
	head->Update(arive,allPos);
	RArm->Update(arive,allPos);
	LArm->Update(arive,allPos);
	body->Update(arive,allPos);
}

void Robot::Update()
{
	SetPRS();
	AllUpdate();
}

void Robot::Draw()
{
	head->Draw(arive);
	RArm->Draw(arive);
	LArm->Draw(arive);
	body->Draw(arive);
}

void Robot::Finalize()
{
	head->Finalize();
	body->Finalize();
	LArm->Finalize();
	RArm->Finalize();
}
