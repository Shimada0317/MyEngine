#include "BossEnemy.h"
#include"Action.h"
#include"EasyMath.h"

using namespace DirectX;
const int HeadDamage = 80;
const int BodyDamage = 50;
const float Subtraction = 0.1f;
const float FallSpeed = 0.25f;
const float AddDefomationValue = 0.04f;
const XMFLOAT4 AddColor = { 0.1f,0.1f,0.1f,0.0f };

BossEnemy::~BossEnemy()
{
	Shadow.reset();
	Center.reset();
	HeadPart.reset();
	BodyPart.reset();
	ArmsPart.reset();

}

void BossEnemy::Initialize(const XMFLOAT3& allrot, const XMVECTOR& allpos, Camera* camera, const XMVECTOR& trackpoint)
{
	AllPos = allpos;

	GetCamera = camera;

	HeadPartRot = BodyPartRot = ArmsPartRot = allrot;
	PursePositiveRot += HeadPartRot.y;
	PurseNegativeeRot += HeadPartRot.y;

	OriginDistance = Distance;
	OriginHeadDistance = HeadDistance;

	Shadow = Object3d::Create(ModelManager::GetInstance()->GetModel(2));
	Center = Object3d::Create(ModelManager::GetInstance()->GetModel(2));
	HeadPart = Object3d::Create(ModelManager::GetInstance()->GetModel(3));
	BodyPart = Object3d::Create(ModelManager::GetInstance()->GetModel(4));
	ArmsPart = Object3d::Create(ModelManager::GetInstance()->GetModel(5));

	PartGreen = ParticleManager::Create(camera);
	PartRed = ParticleManager::Create(camera);

	CenterMat = Center->GetMatrix();
	CenterWorldPos = XMVector3TransformNormal(AllPos, CenterMat);

	Sprite::LoadTexture(40, L"Resources/mark.png");
	RockOn.reset(Sprite::SpriteCreate(40, RockOnPos, RockOnCol, RockOnAnchorPoint));
	RockOnHead.reset(Sprite::SpriteCreate(40, RockOnHeadPos, RockOnCol, RockOnAnchorPoint));

	TrackPoint = OldTrackPoint = trackpoint;

	Hp = 800;
	OldHp = Hp;
	RandomFlag = true;
	TimerLimit = 8;
	RobotAriveFlag = true;
	Center->SetPosition(CenterWorldPos);

}

void BossEnemy::StatusSet()
{
	////変形前なら
	if (DefomationFlag == false) {
		AllPos.m128_f32[1] -= FallSpeed;
		//地面に着いたとき
		if (AllPos.m128_f32[1] <= 2) {
			AllPos.m128_f32[1] = 2;
			DefomationFlag = true;
		}
	}

	Center->SetScale({ 1.0f,1.0f,1.0f });
	XMMatrixIsIdentity(CenterMat);
	CenterMat = Center->GetMatrix();
	CenterWorldPos = XMVector3TransformNormal(AllPos, CenterMat);
	Center->SetPosition(CenterWorldPos);

	ShadowPos = CenterWorldPos;
	ShadowPos.m128_f32[1] = -0.8f;
	Shadow->SetPosition(ShadowPos);
	Shadow->SetRotation({ 0.0f,0.0f,0.0f });
	Shadow->SetScale({ 5.0f,1.0f,5.0f });

	HeadPartPos = ArmsPartPos = BodyPartPos = CenterWorldPos;
	HeadPartPos.m128_f32[1] = CenterWorldPos.m128_f32[1] + 4.0f;
	ArmsPartPos.m128_f32[1] = CenterWorldPos.m128_f32[1] + 0.2f;

	HeadPart->SetPosition(HeadPartPos);
	HeadPart->SetRotation(HeadPartRot);
	HeadPart->SetScale(HeadPartScl);

	BodyPart->SetPosition(BodyPartPos);
	BodyPart->SetRotation(BodyPartRot);
	BodyPart->SetScale(BodyPartScl);

	ArmsPart->SetPosition(ArmsPartPos);
	ArmsPart->SetRotation(ArmsPartRot);
	ArmsPart->SetScale(ArmsPartScl);

	RockOnPos = WorldtoScreen(BodyPartPos);
	RockOnHeadPos = WorldtoScreen(HeadPartPos);
	RockOn->SetPosition(RockOnPos);
	RockOnHead->SetPosition(RockOnHeadPos);
}

void BossEnemy::AllUpdate()
{
	Shadow->Update(ShadowCol);
	Center->Update();

	HeadPart->Update(HeadPartColor);
	BodyPart->Update(BodyPartColor);
	ArmsPart->Update(ArmsPartColor);

	PartRed->Update({ 1.0f,0.0f,0.0f,0.0f });
	PartGreen->Update({ 0.0f,0.5f,0,0.0f });

	for (std::unique_ptr<ObjParticle>& patrticle : Obj_Particle) {
		patrticle->Update();
	}
}

void BossEnemy::Update(const XMFLOAT2& player2Dpos, int& playerhp, bool& playerbulletshot)
{
	Obj_Particle.remove_if([](std::unique_ptr<ObjParticle>& particle) {
		return particle->IsDelete();
		});

	//当たり判定
	if (playerbulletshot == true && Hp > 0) {
		if (player2Dpos.x - Distance * 4 < RockOnPos.x && player2Dpos.x + Distance * 4 > RockOnPos.x &&
			player2Dpos.y - Distance * 4 < RockOnPos.y && player2Dpos.y + Distance * 4 > RockOnPos.y) {
			Hp -= BodyDamage;
			playerbulletshot = false;
		}

		if (player2Dpos.x - HeadDistance * 4 < RockOnHeadPos.x && player2Dpos.x + HeadDistance * 4 > RockOnHeadPos.x &&
			player2Dpos.y - HeadDistance * 4 < RockOnHeadPos.y && player2Dpos.y + HeadDistance * 4 > RockOnHeadPos.y) {
			Hp -= HeadDamage;
			playerbulletshot = false;
		}
	}

	Damage();

	Death();

	if (Hp < 50) {
		AttackTimeMin = 15;
		AttackTimeMax = 20;
	}

	if (RandomFlag == false) {
		TimerLimit = Action::GetInstance()->GetRangRand(AttackTimeMin, AttackTimeMax);
		RandomFlag = true;
	}

	//生きているとき
	if (RobotAriveFlag == true && Hp > 0) {
		if (Length > LengthLimit && DefomationFlag == true) {
			TrackPlayerMode();
		}
		//プレイヤーの前まで来たとき
		else if (Length <= LengthLimit && WaitFlag == false) {
			BodyPartPos.m128_f32[2] -= 1.f;
			AtttackTimer += 0.1f;
			AttackMode(playerhp);
		}
	}
	else {
		AttackFaseFlag = false;
	}

	StatusSet();
	AllUpdate();
}

void BossEnemy::Draw(DirectXCommon* dxCommon)
{
	Sprite::PreDraw(dxCommon->GetCmdList());
	RockOnHead->Draw();
	RockOn->Draw();
	Sprite::PostDraw();

	ParticleManager::PreDraw(dxCommon->GetCmdList());
	PartGreen->Draw();
	PartRed->Draw();
	ParticleManager::PostDraw();

	Object3d::PreDraw(dxCommon->GetCmdList());
	for (std::unique_ptr<ObjParticle>& particle : Obj_Particle) {
		particle->Draw();
	}
	if (Hp > 0) {
		HeadPart->Draw();
		BodyPart->Draw();
		ArmsPart->Draw();
		Shadow->Draw();
	}
	//Center->Draw();
	Object3d::PostDraw();
}

void BossEnemy::TrackPlayerMode()
{
	float vx = 0;
	float vy = 0;
	float vz = 0;

	vx = (AllPos.m128_f32[0] - TrackPoint.m128_f32[0]);
	vy = (AllPos.m128_f32[1] - TrackPoint.m128_f32[1]);
	vz = (AllPos.m128_f32[2] - TrackPoint.m128_f32[2]);

	float v2x = pow(vx, 2);
	float v2y = pow(vy, 2);
	float v2z = pow(vz, 2);
	Length = sqrtf(v2x + v2y + v2z);

	float v3x = (vx / Length) * MoveSpeed;
	float v3y = (vy / Length) * MoveSpeed;
	float v3z = (vz / Length) * MoveSpeed;
	Distance = OriginDistance;
	HeadDistance = OriginHeadDistance;

	Distance -= Length * 2.f;
	HeadDistance -= Length;

	AllPos.m128_f32[0] -= v3x;
	AllPos.m128_f32[2] -= v3z;
}

void BossEnemy::AttackMode(int& playerhp)
{
	if (AtttackTimer >= TimerLimit) {
		AttackFaseFlag = true;
		RandomFlag = false;
	}
	//攻撃フェイズに移行した時
	if (AttackFaseFlag == true) {
		Action::GetInstance()->EaseOut(HeadPartRot.y, PursePositiveRot + 1);
		if (HeadPartRot.y >= PursePositiveRot) {
			HeadPartRot.y = PursePositiveRot;
		}
		Attack(playerhp, AtttackTimer);
	}
	else {
		Action::GetInstance()->EaseOut(HeadPartRot.y, PurseNegativeeRot - 1);
		if (HeadPartRot.y <= PurseNegativeeRot) {
			HeadPartRot.y = PurseNegativeeRot;
		}
	}
}

void BossEnemy::Attack(int& playerhp, float& attacktimer)
{
	//巨大化していく値
	XMFLOAT3 gigantic = { 0.0002f ,0.0002f ,0.0002f };
	float discoloration = 0.01f;
	if (AttackShakeDownFlag == false) {
		ArmsPartRot.x += 1.5f;
		ArmsPartScl = EasyMath::GetInstance()->XMFLOAT3AddXMFLOAT3(ArmsPartScl, gigantic);
		BodyPartScl = EasyMath::GetInstance()->XMFLOAT3AddXMFLOAT3(BodyPartScl, gigantic);
		HeadPartScl = EasyMath::GetInstance()->XMFLOAT3AddXMFLOAT3(HeadPartScl, gigantic);
		ArmsPartColor.y -= discoloration;
		ArmsPartColor.z -= discoloration;
		//腕が最大点に達した時
		if (ArmsPartRot.x >= 40.0f) {
			ArmsPartRot.x = 40;
			if (VibrationChangeFlag == true) {
				Vibration -= 4.2f;
				if (Vibration <= -4.2f) {
					VibrationChangeFlag = false;
				}
			}
			else {
				Vibration += 4.2f;
				if (Vibration >= 4.2f) {
					VibrationChangeFlag = true;
				}
			}
			//体の震え
			HeadPartRot.y += Vibration;
			BodyPartRot.y += Vibration;
			ArmsPartRot.y += Vibration;
			AttackCharge += 0.1f;
			if (AttackCharge >= 10) {
				AttackCharge = 0;
				AttackShakeDownFlag = true;
			}
		}
	}
	else {
		ArmsPartRot.x -= 10.0f;
		if (ArmsPartRot.x <= 0.0f) {
			ArmsPartRot.x = 0.0f;
			ArmsPartColor = { 1.0f,1.0f,1.0f ,1.0f };
			ArmsPartScl = { 0.2f,0.2f,0.2f };
			AttackShakeDownFlag = false;
			AttackFaseFlag = false;
			attacktimer = 0;
			playerhp -= 5;
			Hp = 0;
			SelfDestructingEfect();
		}
	}
}

void BossEnemy::Damage()
{
	//ダメージを受けたとき
	if (OldHp > Hp && Hp >= 0) {
		OldHp = Hp;
		HeadPartColor.y -= 0.2f;
		HeadPartColor.z -= 0.2f;
		BodyPartColor.y -= 0.2f;
		BodyPartColor.z -= 0.2f;
		for (int i = 0; i < 5; i++) {
			std::unique_ptr<ObjParticle> newparticle = std::make_unique<ObjParticle>();
			newparticle->Initialize(1, BodyPartPos, { 1.3f,1.3f,1.3f }, { BodyPartRot });
			Obj_Particle.push_back(std::move(newparticle));
		}
	}
}

void BossEnemy::Death()
{
	//生きているときにHPが0になったら
	if (Hp <= 0) {
		OldTrackPoint = TrackPoint;
		NotLifeFlag = true;
		Hp = 0;
		ShadowCol.w -= Subtraction;
		ArmsPartColor.w -= Subtraction;
		BodyPartColor.w -= Subtraction;
		HeadPartColor.w -= Subtraction;
		if (ParticleEfectFlag == true) {
			//ParticleEfect();
		}
		RobotAriveFlag = false;
		if (Obj_Particle.empty()) {
			DeadFlag = true;
		}
	}
}

XMFLOAT2 BossEnemy::WorldtoScreen(const XMVECTOR& set3Dposition)
{
	Center->SetRotation(CenterRot);
	CenterMat = Center->GetMatrix();
	const float kDistancePlayerTo3DReticle = 50.0f;
	offset = { 0.0,0.0,1.0f };
	offset = XMVector3TransformNormal(offset, CenterMat);
	offset = XMVector3Normalize(offset) * kDistancePlayerTo3DReticle;

	XMVECTOR PositionRet = set3Dposition;

	EasyMath::GetInstance()->ChangeViewPort(MatViewPort, offset);

	XMMATRIX MatVP = MatViewPort;

	XMMATRIX View = GetCamera->GetViewMatrix();
	XMMATRIX Pro = GetCamera->GetProjectionMatrix();

	XMMATRIX MatViewProjectionViewport = View * Pro * MatVP;

	PositionRet = XMVector3TransformCoord(PositionRet, MatViewProjectionViewport);

	XMFLOAT2 get2dposition;
	get2dposition.x = PositionRet.m128_f32[0];
	get2dposition.y = PositionRet.m128_f32[1];

	return get2dposition;
}


void BossEnemy::ParticleEfect()
{
	for (int i = 0; i < 50; i++) {
		XMFLOAT3 pos;

		pos.x = CenterWorldPos.m128_f32[0];
		pos.y = CenterWorldPos.m128_f32[1];
		pos.z = CenterWorldPos.m128_f32[2];

		const float rnd_vel = 0.04f;
		XMFLOAT3 vel{};
		vel.x = Action::GetInstance()->GetRangRand(-0.09f, 0.09f);
		vel.y = Action::GetInstance()->GetRangRand(-0.01f, 0.12f);
		vel.z = Action::GetInstance()->GetRangRand(-0.03f, 0.09f);

		XMFLOAT3 acc{};
		acc.y = 0.0;

		PartRed->Add(400, pos, vel, acc, 120.0f, 0.0f, 150.0f);
		PartGreen->Add(400, pos, vel, acc, 110.1f, 0.0f, 150.0f);
	}
	ParticleEfectFlag = false;
}

void BossEnemy::SelfDestructingEfect()
{
	for (int i = 0; i < 50; i++) {
		XMFLOAT3 pos;

		pos.x = CenterWorldPos.m128_f32[0];
		pos.y = CenterWorldPos.m128_f32[1];
		pos.z = CenterWorldPos.m128_f32[2];

		const float rnd_vel = 0.04f;
		XMFLOAT3 vel{};
		vel.x = Action::GetInstance()->GetRangRand(-0.09f, 0.09f);
		vel.y = Action::GetInstance()->GetRangRand(-0.01f, 0.12f);
		vel.z = Action::GetInstance()->GetRangRand(-0.03f, 0.09f);

		XMFLOAT3 acc{};
		acc.y = 0.0;

		PartRed->Add(400, pos, vel, acc, 120.0f, 0.0f, 150.0f);
		PartGreen->Add(400, pos, vel, acc, 110.1f, 0.0f, 150.0f);
	}
	ParticleEfectFlag = false;
}


