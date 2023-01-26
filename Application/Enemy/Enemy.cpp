#include "Enemy.h"
#include"Action.h"
#include<random>

//�f�X�g���N�^
Enemy::~Enemy()
{
	Shadow.reset();

	HeadPart.reset();
	BodyPart.reset();
	ArmsPart.reset();

	delete HeadPartModel;
	delete BodyPartModel;
	delete ArmsPartModel;
	delete ShadowModel;
	delete ClushSe;
}

//����������
void Enemy::Initialize(const XMFLOAT3& all_Rot, const XMVECTOR& all_Pos, Camera* came, const bool& movement)
{
	HeadPartRot = all_Rot;
	BodyPartRot = all_Rot;
	ArmsPartRot = all_Rot;

	AllPos = all_Pos;
	camera = came;

	ShadowModel = ObjModel::CreateFromOBJ("Shadow", true);
	Shadow = Object3d::Create(ShadowModel);
	Center = Object3d::Create(ShadowModel);


	HeadPartModel = ObjModel::CreateFromOBJ("Head", true);
	HeadPart = Object3d::Create(HeadPartModel);

	BodyPartModel = ObjModel::CreateFromOBJ("tst2", true);
	BodyPart = Object3d::Create(BodyPartModel);

	ArmsPartModel = ObjModel::CreateFromOBJ("BothArm");
	ArmsPart = Object3d::Create(ArmsPartModel);

	CenterMat = Center->GetMatrix();
	CenterWorldPos = XMVector3TransformNormal(AllPos, CenterMat);


	ClushSe = new Audio();
	ClushSe->Initialize();

	Sprite::LoadTexture(350, L"Resources/mark.png");
	RockOn.reset(Sprite::SpriteCreate(350, RockOnPos, RockOnCol, RockOnAnchorPoint));
	RockOnHead.reset(Sprite::SpriteCreate(350, RockOnHeadPos, RockOnCol, RockOnAnchorPoint));

	Hp = 150;
	OldHp = Hp;
	RobotArive = true;
	Center->SetRotation(AllRot);
	Movement_F = movement;
	Center->SetPosition(CenterWorldPos);
}

//�X�e�[�^�X�Z�b�g
void Enemy::StatusSet()
{
	Center->SetScale({ 1.0f,1.0f,1.0f });
	XMMatrixIsIdentity(CenterMat);
	CenterMat = Center->GetMatrix();
	CenterWorldPos = XMVector3TransformNormal(AllPos, CenterMat);
	Center->SetPosition(CenterWorldPos);

	ShadowPos = CenterWorldPos;
	ShadowPos.m128_f32[1] = CenterWorldPos.m128_f32[1] - 0.8f;
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

	RockOnPos=WorldtoScreen(BodyPartPos);
	RockOnHeadPos = WorldtoScreen(HeadPartPos);
	RockOn->SetPosition(RockOnPos);
	RockOnHead->SetPosition(RockOnHeadPos);
}

//Obj���̍X�V�������܂Ƃ߂�
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

//�X�V����
void Enemy::Update(const XMFLOAT2& Player2DPos, int& PlayerHp, bool& PlyerBulletShot)
{
	StatusSet();

	Obj_Particle.remove_if([](std::unique_ptr<ObjParticle>& particle) {
		return particle->IsDelete();

		});

	if (Input::GetInstance()->PushKey(DIK_O)) {
		Hp = 0;
	}

	if (PlyerBulletShot == true) {
		if (Player2DPos.x - Distance < RockOnPos.x && Player2DPos.x + Distance > RockOnPos.x &&
			Player2DPos.y - Distance<RockOnPos.y && Player2DPos.y + Distance>RockOnPos.y) {
			Hp -= 30;
			PlyerBulletShot = false;
		}
	}

	//�_���[�W���󂯂��Ƃ�
	if (OldHp > Hp) {
		ClushSe->LoadFile("Resources/Sound/SE/clush.wav", 0.3f);
		std::unique_ptr<ObjParticle> newparticle = std::make_unique<ObjParticle>();
		newparticle->Initialize();
		Obj_Particle.push_back(std::move(newparticle));
		OldHp = Hp;
	}

	//�����Ă���Ƃ�
	if (RobotArive == true && Hp > 0) {
		TrackPlayerMode();
		//�v���C���[�̑O�܂ŗ����Ƃ�
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

	//�����Ă���Ƃ���HP��0�ɂȂ�����
	if (Hp <= 0) {
		Hp = 0;
		ShadowCol.w -= 0.01f;
		ArmsPartColor.w -= 0.01f;
		BodyPartColor.w -= 0.01f;
		HeadPartColor.w -= 0.01f;
		RobotArive = false;
		AttackPreparationTime = 0;
		AttackChanse = 0;
		Rand = 0;
		if (ArmsPartColor.w <= 0 && Obj_Particle.empty()) {
			isDead_ = true;
		}
	}

	AllUpdate();
}

//�`�揈��
void Enemy::Draw(DirectXCommon* dxCommon)
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
	RockOnHead->Draw();
	Sprite::PostDraw();
}

//�v���C���[�ւ̒ǔ����[�h�̎�
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
	Distance = 60;
	Distance -= Length * 2.0f;

	AllPos.m128_f32[0] -= v3x;
	AllPos.m128_f32[2] -= v3z;
	//�T�C�h�X�e�b�v
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

}

//�U�����[�h�̎�
void Enemy::AttackMode(int& playerHp)
{
	//���Z���ʂ̒l
	int divisionvalue = 0;
	//�U���t�F�C�Y�Ɉڍs���Ă��Ȃ��Ƃ�
	if (AttackFase != true) {
		AttackPreparationTime += 0.01f;
		//�������Ԃ����̒l�ɒB������
		if (AttackPreparationTime >= 12) {
			//0~10�͈̔͂Ȃ̗����𐶐�
			AttackChanse = Action::GetInstance()->GetRangRand(0, 10);
			AttackPreparationTime = 0;
			divisionvalue = AttackChanse % 2;
		}
		//�������������̒l�����̎�
		if (divisionvalue == 1) {
			//�U���Ɉڍs����
			AttackFase = true;
			//�����̏�����
			AttackChanse = 0;
		}
	}
	//�U���t�F�C�Y�Ɉڍs������
	if (AttackFase == true) {
		Attack(playerHp);
	}
}

//�U�����鎞
void Enemy::Attack(int& playerhp)
{
	//���剻���Ă����l
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
			ArmsPartColor = { 1.0f,1.0f,1.0f ,1.0f };
			ArmsPartScl = { 0.2f,0.2f,0.2f };
			AttackShakeDown = false;
			AttackFase = false;
			//playerhp -= 1;
		}
	}
}

//3D���W����2D���W���擾����
XMFLOAT2 Enemy::WorldtoScreen(const XMVECTOR& Set3dPosition)
{
	Center->SetRotation(CenterRot);
	CenterMat = Center->GetMatrix();
	const float kDistancePlayerTo3DReticle = 50.0f;
	offset = { 0.0,0.0,1.0f };
	offset = XMVector3TransformNormal(offset, CenterMat);
	offset = XMVector3Normalize(offset) * kDistancePlayerTo3DReticle;

	XMVECTOR PositionRet = Set3dPosition;

	ChangeViewPort(MatViewPort);

	XMMATRIX MatVP = MatViewPort;

	XMMATRIX View = camera->GetViewMatrix();
	XMMATRIX Pro = camera->GetProjectionMatrix();

	XMMATRIX MatViewProjectionViewport = View * Pro * MatVP;

	PositionRet = XMVector3TransformCoord(PositionRet, MatViewProjectionViewport);

	XMFLOAT2 get2dposition;
	get2dposition.x = PositionRet.m128_f32[0];
	get2dposition.y = PositionRet.m128_f32[1];

	return get2dposition;
}

//�r���[�ϊ�
void Enemy::ChangeViewPort(XMMATRIX& matViewPort)
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
