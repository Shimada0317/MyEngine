#include "Robot.h"
#include"Action.h"
#include<random>


Robot::~Robot()
{
	head.reset();
	body.reset();
	arms.reset();
	Shadow.reset();

	HeadPart.reset();
	BodyPart.reset();
	ArmsPart.reset();


	delete HeadPartModel;
	delete BodyPartModel;
	delete ArmsPartModel;
	delete ShadowModel;
	delete Clush_SE;
}

void Robot::Initialize(const XMFLOAT3& all_Rot, const XMVECTOR& all_Pos,Camera* came, const bool& movement)
{
	HeadPartRot = all_Rot;
	BodyPartRot = all_Rot;
	ArmsPartRot = all_Rot;

	All_Pos = all_Pos;
	camera=came;

	

	ShadowModel = ObjModel::CreateFromOBJ("Shadow",true);
	Shadow = Object3d::Create(ShadowModel);
	Center = Object3d::Create(ShadowModel);

	HeadPartModel = ObjModel::CreateFromOBJ("Head",true);
	HeadPart = Object3d::Create(HeadPartModel);

	BodyPartModel = ObjModel::CreateFromOBJ("tst2",true);
	BodyPart = Object3d::Create(BodyPartModel);

	ArmsPartModel = ObjModel::CreateFromOBJ("BothArm");
	ArmsPart = Object3d::Create(ArmsPartModel);

	Center_Mat = Center->GetMatrix();
	Center_WorldPos = XMVector3TransformNormal(All_Pos, Center_Mat);

	Clush_SE = new Audio();
	Clush_SE->Initialize();

	Sprite::LoadTexture(350, L"Resources/mark.png");
	RockOn.reset(Sprite::SpriteCreate(350, RockOn_Pos, RockOn_Col, RockOn_Anc));

	
	for (int i = 0; i < 3; i++) {
		RemainPart[i] = true;
	}
	Hp = 150;
	OldHp = Hp;
	Robotarive = true;
	Center->SetRotation(All_Rot);
	Movement_F = movement;
	Center->SetPosition(Center_WorldPos);
}

void Robot::StatusSet()
{
	Center->SetScale({ 1.0f,1.0f,1.0f });
	XMMatrixIsIdentity(Center_Mat);
	Center_Mat = Center->GetMatrix();
	Center_WorldPos = XMVector3TransformNormal(All_Pos, Center_Mat);
	Center->SetPosition(Center_WorldPos);

	Shadow_Pos = Center_WorldPos;
	Shadow_Pos.m128_f32[1] = Center_WorldPos.m128_f32[1] - 0.8f;
	Shadow->SetPosition(Shadow_Pos);
	Shadow->SetRotation({ 0.0f,0.0f,0.0f });
	Shadow->SetScale({ 1.0f,1.0f,1.0f });


	HeadPartPos = Center_WorldPos;
	ArmsPartPos = Center_WorldPos;
	BodyPartPos = Center_WorldPos;
	HeadPartPos.m128_f32[1] = Center_WorldPos.m128_f32[1] + 1.0f;
	ArmsPartPos.m128_f32[1] = Center_WorldPos.m128_f32[1] + 0.2f;

	HeadPart->SetPosition(HeadPartPos);
	HeadPart->SetRotation(HeadPartRot);
	HeadPart->SetScale(HeadPartScl);

	BodyPart->SetPosition(BodyPartPos);
	BodyPart->SetRotation(BodyPartRot);
	BodyPart->SetScale(BodyPartScl);

	ArmsPart->SetPosition(ArmsPartPos);
	ArmsPart->SetRotation(ArmsPartRot);
	ArmsPart->SetScale(ArmsPartScl);

	WorldtoScreen();
}

void Robot::AllUpdata()
{

	Shadow->Update(Shadow_Col);
	Center->Update();

	HeadPart->Update(HeadPartColor);
	BodyPart->Update(BodyPartColor);
	ArmsPart->Update(ArmsPartColor);

	for (std::unique_ptr<ObjParticle>& patrticle : Obj_Particle) {
		patrticle->Update(Center_WorldPos, All_Rot);
	}
}

void Robot::Update(const XMFLOAT2& Player2DPos,int& PlayerHp,bool& PlyerBulletShot)
{
	StatusSet();

	Obj_Particle.remove_if([](std::unique_ptr<ObjParticle>& particle) {
		return particle->IsDelete();

		});

	if (Input::GetInstance()->PushKey(DIK_O)) {
		Hp = 0;
	}

	if (PlyerBulletShot == true) {
		if (Player2DPos.x -Distance < RockOn_Pos.x && Player2DPos.x + Distance > RockOn_Pos.x&&
			Player2DPos.y-Distance<RockOn_Pos.y&&Player2DPos.y+Distance>RockOn_Pos.y) {
			Hp -= 30;
			PlyerBulletShot = false;
		}
	}

	//ダメージを受けたとき
	if (OldHp > Hp) {
		Clush_SE->LoadFile("Resources/Sound/SE/clush.wav", 0.3f);
		std::unique_ptr<ObjParticle> newparticle = std::make_unique<ObjParticle>();
		newparticle->Initialize();
		Obj_Particle.push_back(std::move(newparticle));
		OldHp = Hp;
	}

	//生きているとき
	if (Robotarive == true && Hp > 0) {
		TrackPlayerMode();
		//プレイヤーの前まで来たとき
		if (Length <= 2) {
			Motion();
			Movement_F = false;
			MoveSpeed = 0;
			AttackMode(PlayerHp);
		}
	}
	else {
		AttackFase = false;
		TrackPoint.m128_f32[1] = 100;
	}

	//生きているときにHPが0になったら
	if (Hp <= 0) {
		Hp = 0;
		Shadow_Col.w -= 0.01f;
		ArmsPartColor.w -= 0.01f;
		BodyPartColor.w -= 0.01f;
		HeadPartColor.w -= 0.01f;
		Robotarive = false;
		AttackPreparationTime = 0;
		AttackChanse = 0;
		Rand = 0;
		for (int i = 0; i < 3; i++) {
			RemainPart[i] = false;
		}
		if (ArmsPartColor.w <= 0&&Obj_Particle.empty()) {
			isDead_ = true;
		}
	}
	
	AllUpdata();
}

void Robot::Draw(DirectXCommon* dxCommon)
{
	Object3d::PreDraw(dxCommon->GetCmdList());
	HeadPart->Draw();
	BodyPart->Draw();
	ArmsPart->Draw();
	Shadow->Draw();
	//Center->Draw();
	for (std::unique_ptr<ObjParticle>& particle : Obj_Particle) {
		particle->Draw();
	}
	Object3d::PostDraw();

	Sprite::PreDraw(dxCommon->GetCmdList());
	RockOn->Draw();
	Sprite::PostDraw();
}

void Robot::TrackPlayerMode()
{

	float vx = 0;
	float vy = 0;
	float vz = 0;

	vx = (All_Pos.m128_f32[0] - TrackPoint.m128_f32[0]);
	vy = (All_Pos.m128_f32[1] - TrackPoint.m128_f32[1]);
	vz = (All_Pos.m128_f32[2] - TrackPoint.m128_f32[2]);


	float v2x = pow(vx, 2);
	float v2y = pow(vy, 2);
	float v2z = pow(vz, 2);
	Length = sqrtf(v2x + v2y + v2z);

	float v3x = (vx / Length) * MoveSpeed;
	float v3y = (vy / Length) * MoveSpeed;
	float v3z = (vz / Length) * MoveSpeed;
	Distance = 60;
	Distance -= Length*2.0f;

	All_Pos.m128_f32[0] -= v3x;
	All_Pos.m128_f32[2] -= v3z;
	//サイドステップ
	if (Movement_F == true) {
		All_Pos.m128_f32[0] += SideStepSpeed;
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

void Robot::Motion()
{
	//BodyPartRot.y = 30;
	//HeadPartRot.y = 30;
	//ArmsPartRot.y = 30;
}

void Robot::AttackMode(int& playerHp)
{
	//除算結果の値
	int divisionvalue=0;
	//攻撃フェイズに移行していないとき
	if (AttackFase != true) {
		AttackPreparationTime += 0.01f;
		//準備時間が一定の値に達した時
		if (AttackPreparationTime >= 12) {
			//0~10の範囲なの乱数を生成
			AttackChanse=Action::GetInstance()->GetRangRand(0, 10);
			AttackPreparationTime = 0;
			divisionvalue = AttackChanse % 2;
		}
		//生成した乱数の値が一定の時
		if (divisionvalue==1) {
			//攻撃に移行する
			AttackFase = true;
			//乱数の初期化
			AttackChanse = 0;
		}
	}
	//攻撃フェイズに移行した時
	if (AttackFase == true) {
		Attack(playerHp);
	}
}

void Robot::Attack(int& playerhp)
{
	//巨大化していく値
	float gigantic = 0.00002f;
	float discoloration = 0.01f;
	if (AttackShakeDown == false) {
		ArmsPartRot.x += 0.05f;
		ArmsPartScl.x += gigantic;
		ArmsPartScl.y += gigantic;
		ArmsPartScl.z += gigantic;

		ArmsPartColor.y -= discoloration;
		ArmsPartColor.z -= discoloration;
		if (ArmsPartRot.x >= 40.0f) {
			AttackShakeDown = true;
		}
	}
	else {
		ArmsPartRot.x -= 1.0f;
		if (ArmsPartRot.x <= 0.0f) {
			ArmsPartRot.x = 0.0f;
			ArmsPartColor = { 1.0f,1.0f,1.0f ,1.0f};
			ArmsPartScl = { 0.2f,0.2f,0.2f };
			AttackShakeDown = false;
			AttackFase = false;
			//playerhp -= 1;
		}
	}
	


}

void Robot::WorldtoScreen()
{
	Center->SetRotation(Center_Rot);
	Center_Mat = Center->GetMatrix();
	const float kDistancePlayerTo3DReticle = 50.0f;
	offset = { 0.0,0.0,1.0f };
	offset = XMVector3TransformNormal(offset, Center_Mat);
	offset = XMVector3Normalize(offset) * kDistancePlayerTo3DReticle;
	Center_WorldPos = { 0.0f,0.0f,0.0f };
	Center_WorldPos = XMVector3TransformNormal(All_Pos, Center_Mat);
	{
		XMVECTOR PositionRet = Center_WorldPos;

		ChangeViewPort(MatViewPort);

		XMMATRIX MatVP = MatViewPort;

		XMMATRIX View = camera->GetViewMatrix();
		XMMATRIX Pro = camera->GetProjectionMatrix();

		XMMATRIX MatViewProjectionViewport = View * Pro * MatVP;

		PositionRet = XMVector3TransformCoord(PositionRet, MatViewProjectionViewport);

		RockOn_Pos.x = PositionRet.m128_f32[0];
		RockOn_Pos.y = PositionRet.m128_f32[1];

		RockOn->SetPosition(RockOn_Pos);

	}
}

void Robot::ChangeViewPort(XMMATRIX& matViewPort)
{
	matViewPort.r[0].m128_f32[0] = WinApp::window_width / 2;
	matViewPort.r[0].m128_f32[1] = 0;
	matViewPort.r[0].m128_f32[2] = 0;
	matViewPort.r[0].m128_f32[3] = 0;

	matViewPort.r[1].m128_f32[0] = 0;
	matViewPort.r[1].m128_f32[1] = -(WinApp::window_height / 2);
	matViewPort.r[1].m128_f32[2] = 0;
	matViewPort.r[1].m128_f32[3] = 0;

	matViewPort.r[2].m128_f32[0] = 0;
	matViewPort.r[2].m128_f32[1] = 0;
	matViewPort.r[2].m128_f32[2] = 1;
	matViewPort.r[2].m128_f32[3] = 0;

	matViewPort.r[3].m128_f32[0] = WinApp::window_width / 2 + offset.m128_f32[0];
	matViewPort.r[3].m128_f32[1] = WinApp::window_height / 2 + offset.m128_f32[1];
	matViewPort.r[3].m128_f32[2] = 0;
	matViewPort.r[3].m128_f32[3] = 1;
}
