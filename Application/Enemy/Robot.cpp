#include "Robot.h"
#include"Action.h"

Robot::~Robot()
{
	head.reset();
	body.reset();
	Arms.reset();
	part.reset();
	shadow.reset();
	delete shadowModel;
}

void Robot::Initialize(const XMFLOAT3& AllRot)
{
	head = std::make_unique<Head>();
	body = std::make_unique<Body>();
	/*LArm = std::make_unique<LeftArm>();
	RArm = std::make_unique<RightArm>();*/
	Arms = std::make_unique<BothArms>();
	part = std::make_unique<ObjParticle>();
	head->Initialize(Partarive[0],allPos,allRot);
	body->Initialize();
	/*LArm->Initialize();
	RArm->Initialize();*/
	Arms->Initialize();
	part->Initialize();

	shadowModel = ObjModel::CreateFromOBJ("shadow");
	shadow = Object3d::Create(shadowModel);
	

	Arms->RespownSet(AllRot);

	for (int i = 0; i < 3; i++) {
		Partarive[i] = true;
	}
	Hp = 150;
	OldHp = Hp;
	Myarive = true;
	allRot = AllRot;
}

void Robot::AllUpdata(Bullet* bull)
{
	shadowPos = allPos;
	shadowPos.m128_f32[1] = allPos.m128_f32[1] - 0.8f;
	shadow->SetPosition(shadowPos);
	shadow->SetRotation({ 0.0f,0.0f,0.0f });
	shadow->SetScale({ 1.0f,1.0f,1.0f });

	head->Updata(Partarive[0], allPos, allRot, bull, Hp);
	//RArm->Updata(arive[1], allPos, bull, Hp);
	//LArm->Updata(arive[2], allPos, bull, Hp);
	Arms->Updata(Partarive[1], allPos, allRot, bull, Hp);
	body->Updata(Partarive[2], allPos, allRot, bull, Hp);
	part->Updata(allPos, allRot);
	shadow->Updata(shadowColor);
	for (std::unique_ptr<ObjParticle>& patrticle : particle_) {
		patrticle->Updata(allPos, allRot);
	}
}

void Robot::Updata(Bullet* bull, const XMMATRIX& player, bool& spown, int& playerHp)
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
	if (Myarive == true && Hp > 0) {
		TrackPlayer();
		allPos.m128_f32[0] += RobS;
		RobT += 0.1f;
		if (RobT <= 2&&slideF==false) {
			RobS = -RobS;
			RobT = 0;
			slideF = true;
		}
		else if (RobT >= 2 && slideF == true) {
			RobS = +RobS;
			RobT = 0;
			slideF = false;
		}
		if (l <= 2) {
			Motion();
			//プレイヤーの前まで来たとき
			speed = 0;
			if (AttackFase != true) {
				AttackTime += 0.01f;
				if (AttackTime >= 12) {
					rad = (rand() % 10);
					AttackChanse = rad;
					AttackTime = 0;
					rad = 0;
				}
				if (AttackChanse >= 3) {
					AttackFase = true;
					AttackChanse = 0;
				}
			}
			if (AttackFase == true) {
				attackT += 0.1f;

				Arms->Attack(attackT, AttackFase, playerHp,Myarive);
			}
		}
	}
	else {
		AttackFase = false;
		TrackPoint.m128_f32[1] = 100;
	}

	if (Input::GetInstance()->TriggerKey(DIK_O)) {
		Hp = 0;
	}
	
	
	//生きているときにHPが0になったら
	if (Hp <= 0) {
		Hp = 0;
		shadowColor.w = 0.0f;
		Myarive = false;
		//isDead_ = true;
		AttackTime = 0;
		AttackChanse = 0;
		rad = 0;
		for (int i = 0; i < 3; i++) {
			Partarive[i] = false;
		}
		XMFLOAT4 col = body->GetCol();
		if (col.w <=  0&&particle_.empty()) {
			isDead_ = true;
		}

	}

	AllUpdata(bull);
}

void Robot::Draw(DirectXCommon* dxCommon)
{
	Object3d::PreDraw(dxCommon->GetCmdList());
	head->Draw(Partarive[0]);
	/*RArm->Draw(arive[1]);
	LArm->Draw(arive[2]);*/
	Arms->Draw(Partarive[1]);
	body->Draw(Partarive[2]);
	shadow->Draw();
	//part->Draw();
	for (std::unique_ptr<ObjParticle>& particle : particle_) {
		particle->Draw();
	}
	Object3d::PostDraw();
}

void Robot::ImgDraw()
{

	float a = Hp;
	float o = OldHp;
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.7f, 0.7f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.1f, 0.0f, 0.1f, 0.0f));
	ImGui::SetWindowSize(ImVec2(400, 500), ImGuiCond_::ImGuiCond_FirstUseEver);
	ImGui::Begin("Enemy");
	ImGui::Checkbox("Att", &AttackFase);
	ImGui::SliderFloat("Hp", &a, -100.0f, 100.0f);
	ImGui::SliderFloat("len", &l, -100.0f, 100.0f);
	ImGui::SliderFloat("PosZ", &allPos.m128_f32[2], -100.0f, 100.0f);
	ImGui::End();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
}

void Robot::ParticleDraw(DirectXCommon* dxCommon)
{

}

void Robot::TrackPlayer()
{

	float vx = 0;
	float vy = 0;
	float vz = 0;

	vx = (allPos.m128_f32[0] - TrackPoint.m128_f32[0]);
	vy = (allPos.m128_f32[1] - TrackPoint.m128_f32[1]);
	vz = (allPos.m128_f32[2] - TrackPoint.m128_f32[2]);


	float v2x = pow(vx, 2);
	float v2y = pow(vy, 2);
	float v2z = pow(vz, 2);
	l = sqrtf(v2x + v2y + v2z);

	float v3x = (vx / l) * speed;
	float v3y = (vy / l) * speed;
	float v3z = (vz / l) * speed;
	//remaining += 1;
	allPos.m128_f32[0] -= v3x;
	//allPos.m128_f32[1] -= v3y;
	allPos.m128_f32[2] -= v3z;
}

void Robot::Motion()
{
	XMFLOAT3 SclPlus = { 0.00001f,0.00001f,0.00001f };

	XMFLOAT3 bodyScl = body->GetScl();
	XMFLOAT3 headScl = head->GetScl();
	XMFLOAT3 armScl = Arms->GetScl();
	float rot = 0.05f;

		MotionTime += 0.001f;

	if (MotionChange == true) {

		bodyScl.x += SclPlus.x;
		bodyScl.y += SclPlus.y;
		bodyScl.z += SclPlus.z;

		headScl.x += SclPlus.x;
		headScl.y += SclPlus.y;
		headScl.z += SclPlus.z;

		armScl.x += SclPlus.x;
		armScl.y += SclPlus.y;
		armScl.z += SclPlus.z;
		if (MotionTime >= 1) {
			MotionChange = false;
			MotionTime = 0.0f;
		}
	}
	else {
		bodyScl.x -= SclPlus.x;
		bodyScl.y -= SclPlus.y;
		bodyScl.z -= SclPlus.z;

		headScl.x -= SclPlus.x;
		headScl.y -= SclPlus.y;
		headScl.z -= SclPlus.z;

		armScl.x -= SclPlus.x;
		armScl.y -= SclPlus.y;
		armScl.z -= SclPlus.z;
		if (MotionTime >= 1) {
			MotionChange = true;
			MotionTime = 0.0f;
		}
	}

	body->SetScl(bodyScl);
	head->SetScl(headScl);
	Arms->SetScl(armScl);

	head->Motion(rot);

}

void Robot::Finalize()
{

}
