#include "Robot.h"
#include"Action.h"

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
	All_Rot = all_Rot;
	All_Pos = all_Pos;
	camera=came;

	ShadowModel = ObjModel::CreateFromOBJ("Shadow");
	Shadow = Object3d::Create(ShadowModel);
	Center = Object3d::Create(ShadowModel);

	HeadPartModel = ObjModel::CreateFromOBJ("Head");
	HeadPart = Object3d::Create(HeadPartModel);

	BodyPartModel = ObjModel::CreateFromOBJ("tst2");
	BodyPart = Object3d::Create(BodyPartModel);

	ArmsPartModel = ObjModel::CreateFromOBJ("BothArm");
	ArmsPart = Object3d::Create(ArmsPartModel);

	Center_Mat = Center->GetMatrix();
	Center_WorldPos = XMVector3TransformNormal(All_Pos, Center_Mat);

	/*head = std::make_unique<Head>();
	body = std::make_unique<Body>();
	arms = std::make_unique<BothArms>();

	head->Initialize(RemainPart[0],Center_WorldPos,All_Rot,came);
	body->Initialize();
	arms->Initialize();*/


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
	//arms->RespownSet(All_Rot);
	Center->SetRotation(All_Rot);
	Movement_F = movement;
	Center->SetPosition(Center_WorldPos);
}

void Robot::AllUpdata(Bullet* bull)
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

	/*head->Update(RemainPart[0], Center_WorldPos, All_Rot, bull, Hp);
	arms->Update(RemainPart[1], Center_WorldPos, All_Rot, bull, Hp);
	body->Update(RemainPart[2], Center_WorldPos, All_Rot, bull, Hp);*/
	Shadow->Update(Shadow_Col);
	Center->Update();

	HeadPartPos = Center_WorldPos;
	ArmsPartPos = Center_WorldPos;
	BodyPartPos = Center_WorldPos;
	HeadPartPos.m128_f32[1] = Center_WorldPos.m128_f32[1] + 1.0f;
	ArmsPartPos.m128_f32[1] = Center_WorldPos.m128_f32[1] + 0.2f;
	

	HeadPart->SetPosition(HeadPartPos);
	HeadPart->SetRotation(All_Rot);
	HeadPart->SetScale(HeadPartScl);

	BodyPart->SetPosition(BodyPartPos);
	BodyPart->SetRotation(All_Rot);
	BodyPart->SetScale(BodyPartScl);

	ArmsPart->SetPosition(ArmsPartPos);
	ArmsPart->SetRotation(All_Rot);
	ArmsPart->SetScale(ArmsPartScl);

	HeadPart->Update();
	BodyPart->Update();
	ArmsPart->Update();

	WorldtoScreen();

	for (std::unique_ptr<ObjParticle>& patrticle : Obj_Particle) {
		patrticle->Update(Center_WorldPos, All_Rot);
	}
}

void Robot::Update(Bullet* bull,int& playerHp)
{
	Obj_Particle.remove_if([](std::unique_ptr<ObjParticle>& particle) {
		return particle->IsDelete();

		});

	if (Input::GetInstance()->PushKey(DIK_O)) {
		Hp = 0;
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
			//AttackMode(playerHp);
		}
	}
	else {
		AttackFase = false;
		TrackPoint.m128_f32[1] = 100;
	}

	//生きているときにHPが0になったら
	if (Hp <= 0) {
		Hp = 0;
		Shadow_Col.w = 0.0f;
		Robotarive = false;
		AttackPreparationTime = 0;
		AttackChanse = 0;
		Rand = 0;
		for (int i = 0; i < 3; i++) {
			RemainPart[i] = false;
		}
		isDead_ = true;
		//XMFLOAT4 col = body->GetCol();
		/*if (col.w <=  0&&Obj_Particle.empty()) {
			
		}*/

	}
	
	AllUpdata(bull);
}

void Robot::Draw(DirectXCommon* dxCommon)
{
	Object3d::PreDraw(dxCommon->GetCmdList());
	/*head->Draw(RemainPart[0]);
	arms->Draw(RemainPart[1]);
	body->Draw(RemainPart[2]);*/
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

void Robot::ImgDraw()
{

	float a = Hp;
	float o = OldHp;
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.7f, 0.7f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.1f, 0.0f, 0.1f, 0.0f));
	ImGui::SetWindowSize(ImVec2(400, 500), ImGuiCond_::ImGuiCond_FirstUseEver);
	ImGui::Begin("Enemy");
	//ImGui::Checkbox("Att", &AttackFase);
//	ImGui::SliderFloat("Hp", &a, -100.0f, 100.0f);
	//ImGui::SliderFloat("len", &Length, -100.0f, 100.0f);
	//ImGui::SliderFloat("imgPosX", &Ene2DPos.x, -100.0f, 100.0f);
	//ImGui::SliderFloat("imgPosY", &Ene2DPos.y, -100.0f, 100.0f);

	ImGui::SliderFloat("AllPosX", &Center_WorldPos.m128_f32[0], -100.0f, 100.0f);
	ImGui::SliderFloat("AllPosY", &Center_WorldPos.m128_f32[1], -100.0f, 100.0f);
	ImGui::SliderFloat("AllPosZ", &Center_WorldPos.m128_f32[2], -100.0f, 100.0f);

	ImGui::End();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
}

void Robot::ParticleDraw(DirectXCommon* dxCommon)
{

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
	//XMFLOAT3 SclPlus = { 0.00001f,0.00001f,0.00001f };

	//XMFLOAT3 bodyScl = body->GetScl();
	//XMFLOAT3 headScl = head->GetScl();
	//XMFLOAT3 armScl = arms->GetScl();
	//float rot = 0.05f;

	//MotionTime += 0.001f;
	////徐々に大きく
	//if (MotionChange == true) {

	//	bodyScl.x += SclPlus.x;
	//	bodyScl.y += SclPlus.y;
	//	bodyScl.z += SclPlus.z;

	//	headScl.x += SclPlus.x;
	//	headScl.y += SclPlus.y;
	//	headScl.z += SclPlus.z;

	//	armScl.x += SclPlus.x;
	//	armScl.y += SclPlus.y;
	//	armScl.z += SclPlus.z;
	//	if (MotionTime >= 1) {
	//		MotionChange = false;
	//		MotionTime = 0.0f;
	//	}
	//}
	////徐々に小さく
	//else {
	//	bodyScl.x -= SclPlus.x;
	//	bodyScl.y -= SclPlus.y;
	//	bodyScl.z -= SclPlus.z;

	//	headScl.x -= SclPlus.x;
	//	headScl.y -= SclPlus.y;
	//	headScl.z -= SclPlus.z;

	//	armScl.x -= SclPlus.x;
	//	armScl.y -= SclPlus.y;
	//	armScl.z -= SclPlus.z;
	//	if (MotionTime >= 1) {
	//		MotionChange = true;
	//		MotionTime = 0.0f;
	//	}
	//}

	/*body->SetScl(bodyScl);
	head->SetScl(headScl);
	arms->SetScl(armScl);

	head->Motion(rot);*/

}

void Robot::AttackMode(int& playerHp)
{
	//攻撃フェイズに移行していないとき
	if (AttackFase != true) {
		AttackPreparationTime += 0.01f;
		//準備時間が一定の値に達した時
		if (AttackPreparationTime >= 12) {
			Rand = (rand() % 10);
			AttackChanse = Rand;
			AttackPreparationTime = 0;
			Rand = 0;
		}
		//攻撃タイミングが一定以上の値になった時
		if (AttackChanse >= 3) {
			AttackFase = true;
			AttackChanse = 0;
		}
	}
	//攻撃フェイズに移行した時
	if (AttackFase == true) {
		AttackTime += 0.1f;
		arms->Attack(AttackTime, AttackFase, playerHp, Robotarive);
	}
}

void Robot::WorldtoScreen()
{
	const float kDistancePlayerTo3DReticle = 50.0f;
	offset = { 0.0,0.0,1.0f };
	offset = XMVector3TransformNormal(offset, Center_Mat);
	offset = XMVector3Normalize(offset) * kDistancePlayerTo3DReticle;
	Center->SetRotation(Center_Rot);
	Center_Mat = Center->GetMatrix();
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
