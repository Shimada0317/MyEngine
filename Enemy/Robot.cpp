#include "Robot.h"
#include"Action.h"

Robot::~Robot()
{

}

void Robot::Initialize()
{
	head = std::make_unique<Head>();
	body = std::make_unique<Body>();
	/*LArm = std::make_unique<LeftArm>();
	RArm = std::make_unique<RightArm>();*/
	Arms = std::make_unique<BothArms>();
	part = std::make_unique<ObjParticle>();

	head->Initialize();
	body->Initialize();
	/*LArm->Initialize();
	RArm->Initialize();*/
	Arms->Initialize();
	part->Initialize();

	for (int i = 0; i < 3; i++) {
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
	//RArm->Update(arive[1], allPos, bull, Hp);
	//LArm->Update(arive[2], allPos, bull, Hp);
	Arms->Update(arive[1], allPos, bull, Hp);
	body->Update(arive[2], allPos, bull, Hp);
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
	if (all == true && Hp > 0) {
		allPos.m128_f32[2] = allPos.m128_f32[2] - speed;
		//プレイヤーの前まで来たとき
		if (allPos.m128_f32[2] <= playerPos.m128_f32[2] + 4.0f) {
			speed = 0;
			if (AttackTime != true) {
				AttackTime += 0.1f;
			}
			if (AttackTime >= 20) {
				int Attackrand = (rand() % 10);
				AttackChanse = Attackrand;
				AttackTime = 0;
			}
			if (AttackChanse >= 9) {
				AttackFase = true;
			}
			if (AttackFase == true) {
				attackT += 0.1f;
				Arms->Attack(attackT, AttackFase, playerHp);
			}
		}
	}

	//生きているときにHPが0になったら
	if (Hp <= 0) {
		Hp = 50;
		all = false;
		for (int i = 0; i < 3; i++) {
			arive[i] = false;
			allPos = { 0.0f,0.0f,1000000.0f };
		}
	}

	SetPRS(player);
	AllUpdate(bull);
}

void Robot::Draw(DirectXCommon* dxCommon)
{
	Object3d::PreDraw(dxCommon->GetCmdList());
	head->Draw(arive[0]);
	/*RArm->Draw(arive[1]);
	LArm->Draw(arive[2]);*/
	Arms->Draw(arive[1]);
	body->Draw(arive[2]);
	part->Draw();
	Object3d::PostDraw();
}

void Robot::ParticleDraw(DirectXCommon* dxCommon)
{

}

void Robot::AttackAction()
{

}

void Robot::Finalize()
{
	head->Finalize();
	body->Finalize();
	Arms->Finalize();
	//LArm->Finalize();
	//RArm->Finalize();
	part->Finalize();
}

void Robot::SpownEnemy(const XMMATRIX& player, int random)
{

	Hp = 50;
	OldHp = Hp;
	int rad = (rand() % 4);


	float radX = (float)rad;
	for (int i = 0; i < 3; i++) {
		arive[i] = true;
	}
	allPos = { 0.0f,0.0f,0.0f };
	allPos = XMVector3Transform(allPos, player);
	allPos.m128_f32[0] = 0.0f;
	allPos.m128_f32[1] = 0.0f;
	if (rad == 0) {
		allPos.m128_f32[0] = -6.0f;
	}
	else if (rad == 1) {
		allPos.m128_f32[0] = -3.0f;
	}
	else if (rad == 2) {
		allPos.m128_f32[0] = 0.0f;
	}
	else if (rad == 3) {
		allPos.m128_f32[0] = 3.0f;
	}
	else {
		allPos.m128_f32[0] = 6.0f;
	}
	allPos.m128_f32[0] = radX;
	allPos.m128_f32[2] = allPos.m128_f32[2] + 15;
	speed = 0.005f;
}
