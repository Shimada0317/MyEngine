#include "ThrowEnemy.h"
#include"Action.h"

using namespace DirectX;

const int HeadDamage = 80;
const int BodyDamage = 50;
const float Subtraction = 0.05f;
const float FallSpeed = 0.25f;
const float AddDefomationValue = 0.04f;
const XMFLOAT4 AddColor = { 0.1f,0.1f,0.1f,0.0f };

ThrowEnemy::~ThrowEnemy()
{
	Shadow.reset();
	Center.reset();
	HeadPart.reset();
	BodyPart.reset();
	ArmsPart.reset();
}

void ThrowEnemy::Initialize(const XMFLOAT3& allrot, const XMVECTOR& allpos, Camera* camera, const bool& movement)
{
	HeadPartRot = allrot;
	BodyPartRot = allrot;
	ArmsPartRot = allrot;

	AllPos = allpos;
	BringUpCamera = camera;


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

	HeadPartScl = { 0.0f,0.0f,0.0f };
	ArmsPartScl = { 0.0f,0.0f,0.0f };

	Hp = 160;
	OldHp = Hp;
	RobotAriveFlag = true;
	Center->SetRotation(AllRot);
	Center->SetPosition(CenterWorldPos);
}

void ThrowEnemy::StatusSet()
{
	//•ÏŒ`‘O‚È‚ç
	if (DefomationFlag == false) {
		AllPos.m128_f32[1] -= FallSpeed;
		//’n–Ê‚É’…‚¢‚½‚Æ‚«
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

void ThrowEnemy::AllUpdate()
{
	HeadPart->Update();
	BodyPart->Update();
	ArmsPart->Update();
}

void ThrowEnemy::Update()
{


	AllUpdate();
}

void ThrowEnemy::Draw(DirectXCommon* dxCommon)
{
	ParticleManager::PreDraw(dxCommon->GetCmdList());
	PartGreen->Draw();
	PartRed->Draw();
	ParticleManager::PostDraw();

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

void ThrowEnemy::ChangeViewPort(XMMATRIX& matviewport)
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

XMFLOAT2 ThrowEnemy::WorldtoScreen(const XMVECTOR& set3Dposition)
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
