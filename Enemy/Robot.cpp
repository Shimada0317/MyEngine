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
}

void Robot::Update(Bullet* bull, bool& all, const XMMATRIX& player, bool& spown, int& playerHp)
{
	playerPos = { 0.0f,0.0f,0.0f };
	playerPos = XMVector3Transform(playerPos, player);

	//ダメージを受けたとき
	if (OldHp > Hp) {
		part->Effect();

	}

	//生きているとき
	if (all == true  && Hp > 0) {
		allPos.m128_f32[2] = allPos.m128_f32[2] - speed;
		//プレイヤーの前まで来たとき
		if (allPos.m128_f32[2] <= playerPos.m128_f32[2] + 4.0f) {
			speed = 0;
			attackT += 0.5f;
			RArm->Attack(attackT);
			LArm->Attack(attackT);
			if (attackT >= 30.0f) {
				attackT = 0.0f;
				//playerHp -= 1;
			}
		}
	}

	//生きているときにHPが0になったら
	if (Hp <= 0) {
		Hp = 50;
		all = false;
		for (int i = 0; i < 4; i++) {
			arive[i] = false;
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

void Robot::SpownEnemy(const XMMATRIX& player,int random)
{

	Hp = 50;
	OldHp = Hp;
	int rad = (rand() % 4 );
	

	float radX = (float)rad;
	for (int i = 0; i < 4; i++) {
		arive[i] = true;
	}
	allPos = { 0.0f,0.0f,0.0f };
	allPos = XMVector3Transform(allPos, player);
	allPos.m128_f32[0] = 0.0f;
	allPos.m128_f32[1] = 0.0f;
	if (rad == 0) {
		allPos.m128_f32[0] = -2.0f;
	}
	else if (rad == 1) {
		allPos.m128_f32[0] = -1.0f;
	}
	else if (rad == 2) {
		allPos.m128_f32[0] = 0.0f;
	}
	else if(rad == 3) {
		allPos.m128_f32[0] = 1.0f;
	}
	else {
		allPos.m128_f32[0] = 2.0f;
	}
	allPos.m128_f32[0] = radX;
	allPos.m128_f32[2] = allPos.m128_f32[2] + 15;
	speed = 0.005f;
}
