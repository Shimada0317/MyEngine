#include "Robot.h"
#include"Action.h"

void Robot::Initialize()
{
	head = std::make_unique<Head>();
	body = std::make_unique<Body>();
	LArm = std::make_unique<LeftArm>();
	RArm = std::make_unique<RightArm>();
	part = std::make_unique<ObjParticle>();

	head->Initialize();
	body->Initialize();
	LArm->Initialize();
	RArm->Initialize();
	part->Initialize();

	for (int i = 0; i < 4; i++) {
		arive[i] = true;
	}
	OldHp = Hp;
}

void Robot::SetPRS(const XMMATRIX& player)
{
	OldHp = Hp;

}

void Robot::AllUpdate(Bullet* bull)
{
	head->Update(arive[0], allPos, bull, Hp);
	RArm->Update(arive[1], allPos, bull, Hp);
	LArm->Update(arive[2], allPos, bull, Hp);
	body->Update(arive[3], allPos, bull, Hp);
	part->Set(allPos);
	part->Update();
	if (arive[0] == false && arive[1] == false && arive[2] == false && arive[3] == false) {

	}
}

void Robot::Update(Bullet* bull, bool& all, const XMMATRIX& player,bool& spown)
{
	playerPos = XMVector3TransformNormal(playerPos, player);

	//ダメージを受けたとき
	if (OldHp > Hp) {
		part->Effect();

	}

	//生きているとき
	if (all == true && action == 1 && Hp >= 0) {
		allPos.m128_f32[2] = allPos.m128_f32[2] - speed;
		//プレイヤーの前まで来たとき
		if (allPos.m128_f32[2] <= playerPos.m128_f32[2] + 3.0f) {
			speed = 0;
			attackT += 0.5f;
			RArm->Attack(attackT);
			LArm->Attack(attackT);
			if (attackT >= 10.0f) {
				attackT = 0.0f;
			}
		}
		//生きているときにHPが0になったら
		if (Hp <= 0) {
			all = false;
			action = 2;
			for (int i = 0; i < 4; i++) {
				arive[i] = false;
			}
		}
	}


	SetPRS(player);
	AllUpdate(bull);
}

void Robot::Draw(DirectXCommon* dxCommon)
{
	Object3d::PreDraw(dxCommon->GetCmdList());
	head->Draw(arive[0]);
	RArm->Draw(arive[1]);
	LArm->Draw(arive[2]);
	body->Draw(arive[3]);
	part->Draw();
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
	part->Finalize();
}

void Robot::SpownEnemy(const XMMATRIX& player)
{
	Hp = 50;
	OldHp = Hp;
	for (int i = 0; i < 4; i++) {
		arive[i] = true;
	}
	allPos = XMVector3Transform(allPos, player);
	allPos.m128_f32[2] = allPos.m128_f32[2] + 15;
	action = 1;
}
