#include "Enemy.h"
#include"Action.h"
#include<random>

using namespace DirectX;

const int HeadDamage = 40;
const int BodyDamage = 35;
const float Subtraction = 0.05f;
const XMFLOAT4 AddColor = { 0.1f,0.1f,0.1f,0.0f };

const XMFLOAT4 operator+(const DirectX::XMFLOAT4& lhs, const DirectX::XMFLOAT4& rhs)
{
	XMFLOAT4 result;
	result.x = lhs.x + rhs.x;
	result.y = lhs.y + rhs.y;
	result.z = lhs.z + rhs.z;
	result.w = lhs.w + rhs.w;
	return result;
}

//デストラクタ
Enemy::~Enemy()
{
	Shadow.reset();

	HeadPart.reset();
	BodyPart.reset();
	ArmsPart.reset();

	delete ClushSe;
}

//初期化処理
void Enemy::Initialize(const XMFLOAT3& allrot, const XMVECTOR& allpos, Camera* camera, const bool& movement)
{
	HeadPartRot = allrot;
	BodyPartRot = allrot;
	ArmsPartRot = allrot;

	AllPos = allpos;
	BringUpCamera = camera;

	PursePositiveRot += HeadPartRot.y;
	PurseNegativeeRot += HeadPartRot.y;

	OriginDistance = Distance;
	OriginHeadDistance = HeadDistance;

	Shadow = Object3d::Create(ModelManager::GetInstance()->GetModel(2));
	Center = Object3d::Create(ModelManager::GetInstance()->GetModel(2));


	HeadPart = Object3d::Create(ModelManager::GetInstance()->GetModel(3));

	BodyPart = Object3d::Create(ModelManager::GetInstance()->GetModel(4));

	ArmsPart = Object3d::Create(ModelManager::GetInstance()->GetModel(5));

	CenterMat = Center->GetMatrix();
	CenterWorldPos = XMVector3TransformNormal(AllPos, CenterMat);

	Sprite::LoadTexture(350, L"Resources/mark.png");
	RockOn.reset(Sprite::SpriteCreate(350, RockOnPos, RockOnCol, RockOnAnchorPoint));
	RockOnHead.reset(Sprite::SpriteCreate(350, RockOnHeadPos, RockOnCol, RockOnAnchorPoint));

	HeadPartScl = { 0.0f,0.0f,0.0f };
	ArmsPartScl = { 0.0f,0.0f,0.0f };

	Hp = 160;
	OldHp = Hp;
	RobotArive = true;
	Center->SetRotation(AllRot);
	Movement_F = movement;
	Center->SetPosition(CenterWorldPos);
}

//ステータスセット
void Enemy::StatusSet()
{
	//変形前なら
	if (Defomation_F == false) {
		AllPos.m128_f32[1] -= 0.1f;
		//地面に着いたとき
		if (AllPos.m128_f32[1] <= 0) {
			AllPos.m128_f32[1] = 0;
			DefomationCount += 0.01f;
			Action::GetInstance()->EaseOut(HeadPartScl.x, 0.3f);
			Action::GetInstance()->EaseOut(HeadPartScl.y, 0.3f);
			Action::GetInstance()->EaseOut(HeadPartScl.z, 0.3f);

			Action::GetInstance()->EaseOut(ArmsPartScl.x, 0.2f);
			Action::GetInstance()->EaseOut(ArmsPartScl.y, 0.2f);
			Action::GetInstance()->EaseOut(ArmsPartScl.z, 0.2f);
		}
	}

	if (DefomationCount >= 1) {
		DefomationCount = 1;
		Defomation_F = true;
	}
	Center->SetScale({ 1.0f,1.0f,1.0f });
	XMMatrixIsIdentity(CenterMat);
	CenterMat = Center->GetMatrix();
	CenterWorldPos = XMVector3TransformNormal(AllPos, CenterMat);
	Center->SetPosition(CenterWorldPos);

	ShadowPos = CenterWorldPos;
	ShadowPos.m128_f32[1] = -0.8;
	Shadow->SetPosition(ShadowPos);
	Shadow->SetRotation({ 0.0f,0.0f,0.0f });
	Shadow->SetScale({ 1.0f,1.0f,1.0f });

	HeadPartPos = CenterWorldPos;
	ArmsPartPos = CenterWorldPos;
	BodyPartPos = CenterWorldPos;
	HeadPartPos.m128_f32[1] = CenterWorldPos.m128_f32[1] + 1.0f;
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

//Obj等の更新処理をまとめる
void Enemy::AllUpdate()
{
	Shadow->Update(ShadowCol);
	Center->Update();

	HeadPart->Update(HeadPartColor);
	BodyPart->Update(BodyPartColor);
	ArmsPart->Update(ArmsPartColor);

	for (std::unique_ptr<ObjParticle>& patrticle : Obj_Particle) {
		patrticle->Update(CenterWorldPos, AllRot);
	}
}

//更新処理
void Enemy::Update(const XMFLOAT2& player2Dpos, int& playerhp, bool& plyerbulletshot)
{


	Obj_Particle.remove_if([](std::unique_ptr<ObjParticle>& particle) {
		return particle->IsDelete();

		});

	if (plyerbulletshot == true && Hp > 0) {
		if (player2Dpos.x - Distance < RockOnPos.x && player2Dpos.x + Distance > RockOnPos.x &&
			player2Dpos.y - Distance<RockOnPos.y && player2Dpos.y + Distance>RockOnPos.y) {
			Hp -= BodyDamage;
			plyerbulletshot = false;
			for (int i = 0; i < 5; i++) {
				std::unique_ptr<ObjParticle> newparticle = std::make_unique<ObjParticle>();
				newparticle->Initialize(1, BodyPartPos, { 0.3f,0.3f,0.3f }, { BodyPartRot });
				Obj_Particle.push_back(std::move(newparticle));
			}
		}

		if (player2Dpos.x - HeadDistance < RockOnHeadPos.x && player2Dpos.x + HeadDistance > RockOnHeadPos.x &&
			player2Dpos.y - HeadDistance<RockOnHeadPos.y && player2Dpos.y + HeadDistance>RockOnHeadPos.y) {
			Hp -= HeadDamage;
			plyerbulletshot = false;
			for (int i = 0; i < 5; i++) {
				std::unique_ptr<ObjParticle> newparticle = std::make_unique<ObjParticle>();
				newparticle->Initialize(1, BodyPartPos, { 0.3f,0.3f,0.3f }, { BodyPartRot });
				Obj_Particle.push_back(std::move(newparticle));
			}
		}
	}

	//ダメージを受けたとき
	if (OldHp > Hp && Hp >= 0) {
		OldHp = Hp;
	}

	//生きているとき
	if (RobotArive == true && Hp > 0) {
		if (Length > 1.5f&&Defomation_F == true ) {
			TrackPlayerMode();
		}
		//プレイヤーの前まで来たとき
		else if (Length <= 1.5f) {
			Motion();
			Movement_F = false;
			AttackMode(playerhp);
		}
	}
	else {
		AttackFase = false;
		TrackPoint.m128_f32[1] = 100;
	}

	//生きているときにHPが0になったら
	if (Hp <= 0) {
		Hp = 0;
		ShadowCol.w -= Subtraction;
		ArmsPartColor.w -= Subtraction;
		BodyPartColor.w -= Subtraction;
		HeadPartColor.w -= Subtraction;
		RobotArive = false;
		AttackPreparationTime = 0;
		AttackChanse = 0;
		Rand = 0;
		if (ArmsPartColor.w <= 0 && Obj_Particle.empty()) {
			isDead_ = true;
		}
	}

	StatusSet();
	AllUpdate();
}

//描画処理
void Enemy::Draw(DirectXCommon* dxCommon)
{
	Object3d::PreDraw(dxCommon->GetCmdList());
	for (std::unique_ptr<ObjParticle>& particle : Obj_Particle) {
		particle->Draw();
	}
	HeadPart->Draw();
	BodyPart->Draw();
	ArmsPart->Draw();
	Shadow->Draw();
	//Center->Draw();

	Object3d::PostDraw();
}

//プレイヤーへの追尾モードの時
void Enemy::TrackPlayerMode()
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

	Distance -= Length * 2.0f;
	HeadDistance -= Length;

	AllPos.m128_f32[0] -= v3x;
	AllPos.m128_f32[2] -= v3z;
	//サイドステップ
	if (Movement_F == true) {
		AllPos.m128_f32[0] += SideStepSpeed;
		MovementChangeTime += 0.01f;
		if (MovementChangeTime <= 2 && Reversal_F == false) {
			SideStepSpeed = -SideStepSpeed;
			MovementChangeTime = 0;
			Reversal_F = true;
		}
		else if (MovementChangeTime >= 2 && Reversal_F == true) {
			SideStepSpeed = +SideStepSpeed;
			MovementChangeTime = 0;
			Reversal_F = false;
		}
	}
}

void Enemy::Motion()
{
	XMFLOAT3 hypertrophy = { 0.00001f,0.00001f,0.00001f };

	MotionTime += 0.0001f;

	if (MotionChange == true) {
		BodyPartScl.x += hypertrophy.x;
		ArmsPartScl.x += hypertrophy.x;
		HeadPartScl.x += hypertrophy.x;

		BodyPartScl.y += hypertrophy.y;
		ArmsPartScl.y += hypertrophy.y;
		HeadPartScl.y += hypertrophy.y;

		BodyPartScl.z += hypertrophy.z;
		ArmsPartScl.z += hypertrophy.z;
		HeadPartScl.z += hypertrophy.z;
		if (MotionTime >= 1) {
			MotionChange = false;
			MotionTime = 0.0f;
		}
	}

	else {
		BodyPartScl.x -= hypertrophy.x;
		ArmsPartScl.x -= hypertrophy.x;
		HeadPartScl.x -= hypertrophy.x;

		BodyPartScl.y -= hypertrophy.y;
		ArmsPartScl.y -= hypertrophy.y;
		HeadPartScl.y -= hypertrophy.y;

		BodyPartScl.z -= hypertrophy.z;
		ArmsPartScl.z -= hypertrophy.z;
		HeadPartScl.z -= hypertrophy.z;
		if (MotionTime >= 1) {
			MotionChange = true;
			MotionTime = 0.0f;
		}
	}

}

//攻撃モードの時
void Enemy::AttackMode(int& playerhp)
{
	//除算結果の値
	int divisionvalue = 0;
	//攻撃フェイズに移行していないとき
	if (AttackFase != true) {
		AttackPreparationTime += 0.1f;
		//準備時間が一定の値に達した時
		if (AttackPreparationTime >= 12) {
			//0~10の範囲なの乱数を生成
			AttackChanse = Action::GetInstance()->GetRangRand(0, 10);
			AttackPreparationTime = 0;
			divisionvalue = AttackChanse % 2;
		}
		//生成した乱数の値が一定の時
		if (divisionvalue == 1) {

			//攻撃に移行する
			AttackFase = true;
			//乱数の初期化
			AttackChanse = 0;
		}
	}
	//攻撃フェイズに移行した時
	if (AttackFase == true) {
		Action::GetInstance()->EaseOut(HeadPartRot.y, PursePositiveRot + 1);
		if (HeadPartRot.y >= PursePositiveRot) {
			HeadPartRot.y = PursePositiveRot;
		}
		Attack(playerhp);
	}
	else {
		Action::GetInstance()->EaseOut(HeadPartRot.y, PurseNegativeeRot - 1);
		if (HeadPartRot.y <= PurseNegativeeRot) {
			HeadPartRot.y = PurseNegativeeRot;
		}

	}
}

//攻撃する時
void Enemy::Attack(int& playerhp)
{
	//巨大化していく値
	float gigantic = 0.00002f;
	float discoloration = 0.01f;
	if (AttackShakeDown == false) {
		ArmsPartRot.x += 1.5f;
		ArmsPartScl.x += gigantic;
		ArmsPartScl.y += gigantic;
		ArmsPartScl.z += gigantic;

		ArmsPartColor.y -= discoloration;
		ArmsPartColor.z -= discoloration;
		//腕が最大点に達した時
		if (ArmsPartRot.x >= 40.0f) {
			ArmsPartRot.x = 40;

			if (VibrationChange == true) {
				Vibration -= 2.2f;
				if (Vibration <= -2.2f) {
					VibrationChange = false;
				}
			}
			else {
				Vibration += 2.2f;
				if (Vibration >= 2.2f) {
					VibrationChange = true;
				}
			}
			//体の震え
			HeadPartRot.y += Vibration;
			BodyPartRot.y += Vibration;
			ArmsPartRot.y += Vibration;
			AttackCharge += 0.1f;
			if (AttackCharge >= 10) {
				AttackCharge = 0;
				AttackShakeDown = true;
			}
		}
	}
	else {
		ArmsPartRot.x -= 10.0f;
		if (ArmsPartRot.x <= 0.0f) {
			ArmsPartRot.x = 0.0f;
			ArmsPartColor = { 1.0f,1.0f,1.0f ,1.0f };
			ArmsPartScl = { 0.2f,0.2f,0.2f };
			AttackShakeDown = false;
			AttackFase = false;
			playerhp -= 1;
		}
	}
}

//3D座標から2D座標を取得する
XMFLOAT2 Enemy::WorldtoScreen(const XMVECTOR& set3Dposition)
{
	Center->SetRotation(CenterRot);
	CenterMat = Center->GetMatrix();
	const float kDistancePlayerTo3DReticle = 50.0f;
	offset = { 0.0,0.0,1.0f };
	offset = XMVector3TransformNormal(offset, CenterMat);
	offset = XMVector3Normalize(offset) * kDistancePlayerTo3DReticle;

	XMVECTOR PositionRet = set3Dposition;

	ChangeViewPort(MatViewPort);

	XMMATRIX MatVP = MatViewPort;

	XMMATRIX View = BringUpCamera->GetViewMatrix();
	XMMATRIX Pro = BringUpCamera->GetProjectionMatrix();

	XMMATRIX MatViewProjectionViewport = View * Pro * MatVP;

	PositionRet = XMVector3TransformCoord(PositionRet, MatViewProjectionViewport);

	XMFLOAT2 get2dposition;
	get2dposition.x = PositionRet.m128_f32[0];
	get2dposition.y = PositionRet.m128_f32[1];

	return get2dposition;
}

//ビュー変換
void Enemy::ChangeViewPort(XMMATRIX& matviewport)
{
	matviewport.r[0].m128_f32[0] = WinApp::window_width / 2;
	matviewport.r[0].m128_f32[1] = 0;
	matviewport.r[0].m128_f32[2] = 0;
	matviewport.r[0].m128_f32[3] = 0;
	   
	matviewport.r[1].m128_f32[0] = 0;
	matviewport.r[1].m128_f32[1] = -(WinApp::window_height / 2);
	matviewport.r[1].m128_f32[2] = 0;
	matviewport.r[1].m128_f32[3] = 0;
	   
	matviewport.r[2].m128_f32[0] = 0;
	matviewport.r[2].m128_f32[1] = 0;
	matviewport.r[2].m128_f32[2] = 1;
	matviewport.r[2].m128_f32[3] = 0;
	   
	matviewport.r[3].m128_f32[0] = WinApp::window_width / 2 + offset.m128_f32[0];
	matviewport.r[3].m128_f32[1] = WinApp::window_height / 2 + offset.m128_f32[1];
	matviewport.r[3].m128_f32[2] = 0;
	matviewport.r[3].m128_f32[3] = 1;
}
