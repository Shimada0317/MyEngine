#include "Robot.h"
#include"Action.h"

Robot::~Robot()
{
	head.reset();
	body.reset();
	Arms.reset();
	part.reset();
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
	

}

void Robot::AllUpdate(Bullet* bull)
{
	head->Update(arive[0], allPos, bull, Hp);
	//RArm->Update(arive[1], allPos, bull, Hp);
	//LArm->Update(arive[2], allPos, bull, Hp);
	Arms->Update(arive[1], allPos, bull, Hp);
	body->Update(arive[2], allPos, bull, Hp);
	part->Update(allPos);
	for (std::unique_ptr<ObjParticle>& patrticle : particle_) {
		patrticle->Update(allPos);
	}
}

void Robot::Update(Bullet* bull, bool& all, const XMMATRIX& player, bool& spown, int& playerHp)
{
	particle_.remove_if([](std::unique_ptr<ObjParticle>& particle) {
		return particle->IsDelete();

		});


	playerPos = { 0.0f,0.0f,0.0f };
	playerPos = XMVector3Transform(playerPos, player);

	//ダメージを受けたとき
	if (OldHp > Hp) {
		//part->Effect();
		std::unique_ptr<ObjParticle> newparticle = std::make_unique<ObjParticle>();
		newparticle->Initialize();
 		particle_.push_back(std::move(newparticle));
		OldHp = Hp;
	}

	//生きているとき
	if (all == true && Hp > 0) {
		allPos.m128_f32[2] = allPos.m128_f32[2] - speed;
		//プレイヤーの前まで来たとき
		if ((allPos.m128_f32[0] <= playerPos.m128_f32[0] + 6.0f && allPos.m128_f32[0] >= playerPos.m128_f32[0] - 6.0f) 
			&&(allPos.m128_f32[1] <= playerPos.m128_f32[1] + 6.0f && allPos.m128_f32[1]>=playerPos.m128_f32[1]-6.0f)
			&& (allPos.m128_f32[2] <= playerPos.m128_f32[2] + 3.0f && allPos.m128_f32[2] >= playerPos.m128_f32[2] - 3.0f)) {
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
	if (Input::GetInstance()->TriggerKey(DIK_O)) {
		Hp=0;
	}
	//生きているときにHPが0になったら
	if (Hp <= 0) {
		Hp = 1;
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
	//part->Draw();
	for (std::unique_ptr<ObjParticle>& particle : particle_) {
		particle->Draw();
	}
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

}

void Robot::SpownEnemy(const XMMATRIX& player, int patern)
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
	if (patern <= 2) {
		allPos.m128_f32[0] = 0.0f;
		allPos.m128_f32[1] = 0.0f;
		allPos.m128_f32[2] = allPos.m128_f32[2] + 15;
	}
	else if (patern == 3 || patern == 4) {
		allPos.m128_f32[1] = 0.0f;
		allPos.m128_f32[0] = allPos.m128_f32[0] + 15;
		//allPos.m128_f32[2] = 40;
	}
	else if (patern >= 5 && patern <= 6) {
		allPos.m128_f32[1] = 0.0f;
		allPos.m128_f32[2] = allPos.m128_f32[2] - 15;
	}
	else if (patern == 7 ) {
		allPos.m128_f32[1] = 0.0f;
		allPos.m128_f32[0] = allPos.m128_f32[0] - 15;
	}
	else if (patern == 8) {
		allPos.m128_f32[0] = 0.0f;
		allPos.m128_f32[1] = 0.0f;
		allPos.m128_f32[2] = allPos.m128_f32[2] + 15;
	}
	if (patern <= 3) {
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
		allPos.m128_f32[2] = allPos.m128_f32[2] + 15;
	}
	else if (patern > 3 && patern <= 5) {
		allPos.m128_f32[2] = 40.0f;
		allPos.m128_f32[1] = 0.0f;
		if (rad == 0) {
			allPos.m128_f32[2] -= 6.0f;
		}
		else if (rad == 1) {
			allPos.m128_f32[2] -= 3.0f;
		}
		else if (rad == 2) {
			allPos.m128_f32[2] = 0.0f;
		}
		else if (rad == 3) {
			allPos.m128_f32[2] += 3.0f;
		}
		else {
			allPos.m128_f32[2] += 6.0f;
		}
		allPos.m128_f32[0] = allPos.m128_f32[0] + 15;
	}
	speed = 0.005f;
}
