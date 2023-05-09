#include "NormalEnemy.h"
#include"Action.h"
#include"HelperMath.h"
#include"SpriteManager.h"

using namespace DirectX;
const int HeadDamage = 80;
const int BodyDamage = 50;
const float Subtraction = 0.1f;
const float FallSpeed = 0.25f;
const float AddDefomationValue = 0.04f;
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

//�f�X�g���N�^
NormalEnemy::~NormalEnemy()
{
	Shadow.reset();
	Center.reset();
	HeadPart.reset();
	BodyPart.reset();
	ArmsPart.reset();

	delete ClushSe;
}

//����������
void NormalEnemy::Initialize(const XMFLOAT3& allrot, const XMVECTOR& allpos, Camera* camera, const XMVECTOR& trackpoint, const bool& movement)
{
	HeadPartRot =BodyPartRot = ArmsPartRot = allrot;

	AllPos = allpos;
	BringUpCamera = camera;

	PursePositiveRot += HeadPartRot.y;
	PurseNegativeeRot += HeadPartRot.y;

	OriginDistance = Distance;
	OriginHeadDistance = HeadDistance;

	Shadow = Object3d::Create(ModelManager::GetInstance()->GetModel(kShadow));
	Center = Object3d::Create(ModelManager::GetInstance()->GetModel(kShadow));
	HeadPart = Object3d::Create(ModelManager::GetInstance()->GetModel(kHead));
	BodyPart = Object3d::Create(ModelManager::GetInstance()->GetModel(kBody));
	ArmsPart = Object3d::Create(ModelManager::GetInstance()->GetModel(kArms));

	PartGreen = ParticleManager::Create(BringUpCamera);
	PartRed = ParticleManager::Create(BringUpCamera);

	CenterMat = Center->GetMatrix();
	CenterWorldPos = XMVector3TransformNormal(AllPos, CenterMat);

	RockOn.reset(Sprite::SpriteCreate(Name::kEnemyMarker, RockOnPos, RockOnCol, RockOnAnchorPoint));
	RockOnHead.reset(Sprite::SpriteCreate(Name::kEnemyMarker, RockOnHeadPos, RockOnCol, RockOnAnchorPoint));

	HeadPartScl = { 0.0f,0.0f,0.0f };
	ArmsPartScl = { 0.0f,0.0f,0.0f };

	TrackPoint = OldTrackPoint = trackpoint;

	Hp = 160;
	OldHp = Hp;
	RandomFlag = true;
	TimerLimit = 8;
	RobotAriveFlag = true;
	MovementFlag = movement;
	Center->SetPosition(CenterWorldPos);
}

//�X�e�[�^�X�Z�b�g
void NormalEnemy::StatusSet()
{
	
	Center->SetScale({ 1.0f,1.0f,1.0f });
	XMMatrixIsIdentity(CenterMat);
	CenterMat = Center->GetMatrix();
	CenterWorldPos = XMVector3TransformNormal(AllPos, CenterMat);
	Center->SetPosition(CenterWorldPos);

	ShadowPos = CenterWorldPos;
	ShadowPos.m128_f32[1] = -0.8f;
	Shadow->SetPosition(ShadowPos);
	Shadow->SetRotation({ 0.0f,0.0f,0.0f });
	Shadow->SetScale({ 1.0f,1.0f,1.0f });

	HeadPartPos = ArmsPartPos = BodyPartPos = CenterWorldPos;
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

//Obj���̍X�V�������܂Ƃ߂�
void NormalEnemy::AllUpdate()
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

//�X�V����
void NormalEnemy::Update(const XMFLOAT2& player2Dpos, int& playerhp, bool& playerbulletshot)
{
	//�ό`
	Defomation();
	
	Obj_Particle.remove_if([](std::unique_ptr<ObjParticle>& particle) {
		return particle->IsDelete();

		});

	//�����蔻��
	if (playerbulletshot == true && Hp > 0) {
		if (player2Dpos.x - Distance*1.3 < RockOnPos.x && player2Dpos.x + Distance * 1.3 > RockOnPos.x &&
			player2Dpos.y - Distance * 1.3 <RockOnPos.y && player2Dpos.y + Distance * 1.3 >RockOnPos.y) {
			Hp -= BodyDamage;
			playerbulletshot = false;
		}

		if (player2Dpos.x - HeadDistance * 1.3 < RockOnHeadPos.x && player2Dpos.x + HeadDistance * 1.3 > RockOnHeadPos.x &&
			player2Dpos.y - HeadDistance * 1.3 <RockOnHeadPos.y && player2Dpos.y + HeadDistance * 1.3 >RockOnHeadPos.y) {
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

	//�����Ă���Ƃ�
	if (RobotAriveFlag == true && Hp > 0) {
		if (Length > LengthLimit && DefomationFlag == true) {
			TrackPlayerMode();
		}
		//�v���C���[�̑O�܂ŗ����Ƃ�
		else if (Length <= LengthLimit&&WaitFlag==false) {
			BodyPartPos.m128_f32[2] -= 1.f;
			AtttackTimer += 0.1f;
			MovementFlag = false;
			AttackMode(playerhp);
		}
	}
	else {
		AttackFaseFlag = false;
	}

	if (Input::GetInstance()->PushKey(DIK_O)) {
		Hp = 0;
	}

	StatusSet();
	AllUpdate();
}

//�`�揈��
void NormalEnemy::Draw(DirectXCommon* dxCommon)
{
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

void NormalEnemy::Defomation()
{
	//�ό`�O�Ȃ�
	if (DefomationFlag == false) {
		AllPos.m128_f32[1] -= FallSpeed;
		//�n�ʂɒ������Ƃ�
		if (AllPos.m128_f32[1] <= 0) {
			AllPos.m128_f32[1] = 0;
			DefomationCount += AddDefomationValue;
			if (HeadPartScl.z <= 0.3f && ArmsPartScl.z <= 0.2f) {
				Action::GetInstance()->EaseOut(HeadPartScl.x, 1.0f);
				Action::GetInstance()->EaseOut(HeadPartScl.y, 1.0f);
				Action::GetInstance()->EaseOut(HeadPartScl.z, 1.0f);

				Action::GetInstance()->EaseOut(ArmsPartScl.x, 0.8f);
				Action::GetInstance()->EaseOut(ArmsPartScl.y, 0.8f);
				Action::GetInstance()->EaseOut(ArmsPartScl.z, 0.8f);
			}
		}
	}

	if (DefomationCount >= 1) {
		DefomationCount = 1;
		DefomationFlag = true;
	}
}

//�v���C���[�ւ̒ǔ����[�h�̎�
void NormalEnemy::TrackPlayerMode()
{
	float vx = 0;
	float vy = 0;
	float vz = 0;

	vx = (AllPos.m128_f32[0] - TrackPoint.m128_f32[0]);
	vy = (AllPos.m128_f32[1] - TrackPoint.m128_f32[1]);
	vz = (AllPos.m128_f32[2] - TrackPoint.m128_f32[2]);

	float v2x = powf(vx, 2.f);
	float v2y = powf(vy, 2.f);
	float v2z = powf(vz, 2.f);
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
	
}

//�U�����[�h�̎�
void NormalEnemy::AttackMode(int& playerhp)
{
	if (AtttackTimer >= TimerLimit) {
		AttackFaseFlag = true;
		RandomFlag = false;
	}
	
	//�U���t�F�C�Y�Ɉڍs������
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

//�U�����鎞
void NormalEnemy::Attack(int& playerhp, float& attacktimer)
{
	//���剻���Ă����l
	XMFLOAT3 gigantic = { 0.0002f ,0.0002f ,0.0002f };
	float discoloration = 0.01f;
	if (AttackShakeDownFlag == false) {
		ArmsPartRot.x += 1.5f;
		ArmsPartScl=HelperMath::GetInstance()->XMFLOAT3AddXMFLOAT3(ArmsPartScl, gigantic);
		BodyPartScl=HelperMath::GetInstance()->XMFLOAT3AddXMFLOAT3(BodyPartScl, gigantic);
		HeadPartScl=HelperMath::GetInstance()->XMFLOAT3AddXMFLOAT3(HeadPartScl, gigantic);
		ArmsPartColor.y -= discoloration;
		ArmsPartColor.z -= discoloration;
		//�r���ő�_�ɒB������
		if (ArmsPartRot.x >= 40.0f) {
			ArmsPartRot.x = 40;
			LengthLimit = 0.1f;
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
			//�̂̐k��
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
			playerhp -= 1;
			Hp = 0;

		}
	}
}

void NormalEnemy::Damage()
{
	//�_���[�W���󂯂��Ƃ�
	if (OldHp > Hp && Hp >= 0) {
		OldHp = Hp;
		HeadPartColor.y -= 0.2f;
		HeadPartColor.z -= 0.2f;
		BodyPartColor.y -= 0.2f;
		BodyPartColor.z -= 0.2f;
		for (int i = 0; i < 5; i++) {
			std::unique_ptr<ObjParticle> newparticle = std::make_unique<ObjParticle>();
			newparticle->Initialize(1, BodyPartPos, { 0.3f,0.3f,0.3f }, { BodyPartRot });
			Obj_Particle.push_back(std::move(newparticle));
		}
	}
}

void NormalEnemy::Death()
{
	//�����Ă���Ƃ���HP��0�ɂȂ�����
	if (Hp <= 0) {
		OldTrackPoint = TrackPoint;
		NotLifeFlag = true;
		Hp = 0;
		ShadowCol.w -= Subtraction;
		ArmsPartColor.w -= Subtraction;
		BodyPartColor.w -= Subtraction;
		HeadPartColor.w -= Subtraction;
		if (ParticleEfectFlag == true) {
			ParticleEfect();
		}
		RobotAriveFlag = false;


		if (Obj_Particle.empty()) {
			DeadFlag = true;
		}
	}
}

//3D���W����2D���W���擾����
XMFLOAT2 NormalEnemy::WorldtoScreen(const XMVECTOR& set3Dposition)
{
	Center->SetRotation(CenterRot);
	CenterMat = Center->GetMatrix();
	const float kDistancePlayerTo3DReticle = 50.0f;
	offset = { 0.0,0.0,1.0f };
	offset = XMVector3TransformNormal(offset, CenterMat);
	offset = XMVector3Normalize(offset) * kDistancePlayerTo3DReticle;

	XMVECTOR PositionRet = set3Dposition;

	HelperMath::GetInstance()->ChangeViewPort(MatViewPort,offset);

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


void NormalEnemy::ParticleEfect()
{
	for (int i = 0; i < 50; i++) {
		XMFLOAT3 pos;

		pos.x = CenterWorldPos.m128_f32[0];
		pos.y = CenterWorldPos.m128_f32[1];
		pos.z = CenterWorldPos.m128_f32[2];

		const float rnd_vel = 0.04f;
		XMFLOAT3 vel{};
		vel.x = Action::GetInstance()->GetRangRand(-0.09f,0.09f);
		vel.y = Action::GetInstance()->GetRangRand(-0.01f, 0.12f);
		vel.z = Action::GetInstance()->GetRangRand(-0.03f, 0.09f);

		XMFLOAT3 acc{};
		acc.y = 0.0;

		PartRed->Add(200, pos, vel, acc, 4.0f, 0.0f, 150.0f);
		PartGreen->Add(200, pos, vel, acc, 3.7f, 0.0f, 150.0f);
	}
	ParticleEfectFlag = false;

}

void NormalEnemy::WaitTrack(bool otherenemyarive)
{
	if (otherenemyarive == true) {
		LengthLimit = 2.5f;
		OldTrackPoint.m128_f32[2] = OldTrackPoint.m128_f32[2] - 2;
		WaitFlag = true;
	}
	else {
		LengthLimit = 1.5f;
		OldTrackPoint = TrackPoint;
		WaitFlag = false;
		AtttackTimer = 0;
	}

}