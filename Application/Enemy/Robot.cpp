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

void Robot::Initialize(const XMFLOAT3& AllRot, const XMVECTOR& AllPos,Camera* came, const bool& Step)
{
	shadowModel = ObjModel::CreateFromOBJ("shadow");
	shadow = Object3d::Create(shadowModel);
	center = Object3d::Create(shadowModel);

	allPos = AllPos;
	cmat = center->GetMatrix();
	alll = XMVector3TransformNormal(allPos, cmat);
	camera=came;

	head = std::make_unique<Head>();
	body = std::make_unique<Body>();
	/*LArm = std::make_unique<LeftArm>();
	RArm = std::make_unique<RightArm>();*/
	Arms = std::make_unique<BothArms>();
	part = std::make_unique<ObjParticle>();
	head->Initialize(Partarive[0],alll,allRot);
	body->Initialize();
	/*LArm->Initialize();
	RArm->Initialize();*/
	Arms->Initialize();
	part->Initialize();

	

	Sprite::LoadTexture(350, L"Resources/mark.png");
	RockOn.reset(Sprite::SpriteCreate(350, RockOnPos, RockOnCol, RockOnAnc));
	

	Arms->RespownSet(AllRot);

	for (int i = 0; i < 3; i++) {
		Partarive[i] = true;
	}
	Hp = 150;
	OldHp = Hp;
	Myarive = true;
	allRot = AllRot;
	center->SetRotation(AllRot);
	step = Step;


	center->SetPosition(alll);
}

void Robot::AllUpdata(Bullet* bull)
{
	//XMMATRIX cmat = XMMatrixIdentity();
	//center->SetPosition(allPos);
	//center->SetRotation(allRot);
	

	center->SetScale({ 1.0f,1.0f,1.0f });
	//center->SetRotation({ allRot });
	XMMatrixIsIdentity(cmat);
	cmat = center->GetMatrix();
	alll = XMVector3TransformNormal(allPos, cmat);
	center->SetPosition(alll);

	shadowPos = alll;
	shadowPos.m128_f32[1] = alll.m128_f32[1] - 0.8f;
	shadow->SetPosition(shadowPos);
	shadow->SetRotation({ 0.0f,0.0f,0.0f });
	shadow->SetScale({ 1.0f,1.0f,1.0f });

	head->Updata(Partarive[0], alll, allRot, bull, Hp);
	//RArm->Updata(arive[1], allPos, bull, Hp);
	//LArm->Updata(arive[2], allPos, bull, Hp);
	Arms->Updata(Partarive[1], alll, allRot, bull, Hp);
	body->Updata(Partarive[2], alll, allRot, bull, Hp);
	part->Updata(alll, allRot);
	shadow->Updata(shadowColor);
	center->Updata();

	WorldtoScreen();

	for (std::unique_ptr<ObjParticle>& patrticle : particle_) {
		patrticle->Updata(alll, allRot);
	}
}

void Robot::Updata(Bullet* bull, const XMMATRIX& player, bool& spown, int& playerHp)
{
	particle_.remove_if([](std::unique_ptr<ObjParticle>& particle) {
		return particle->IsDelete();

		});

	playerPos = { 0.0f,0.0f,0.0f };
	playerPos = XMVector3Transform(playerPos, player);

	if (Input::GetInstance()->TriggerKey(DIK_O)) {
		Hp = 0;
	}

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
		if (l <= 2) {
			step = false;
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

	//生きているときにHPが0になったら
	if (Hp <= 0) {
		Hp = 0;
		shadowColor.w = 0.0f;
		Myarive = false;
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
	center->Draw();
	//part->Draw();
	for (std::unique_ptr<ObjParticle>& particle : particle_) {
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
	//ImGui::SliderFloat("len", &l, -100.0f, 100.0f);
	//ImGui::SliderFloat("imgPosX", &Ene2DPos.x, -100.0f, 100.0f);
	//ImGui::SliderFloat("imgPosY", &Ene2DPos.y, -100.0f, 100.0f);

	ImGui::SliderFloat("AllPosX", &alll.m128_f32[0], -100.0f, 100.0f);
	ImGui::SliderFloat("AllPosY", &alll.m128_f32[1], -100.0f, 100.0f);
	ImGui::SliderFloat("AllPosZ", &alll.m128_f32[2], -100.0f, 100.0f);

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

	if (step == true) {
		allPos.m128_f32[0] += RobS;
		RobT += 0.01f;
		if (RobT <= 2 && slideF == false) {
			RobS = -RobS;
			RobT = 0;
			slideF = true;
		}
		else if (RobT >= 2 && slideF == true) {
			RobS = +RobS;
			RobT = 0;
			slideF = false;
		}
	}
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

void Robot::WorldtoScreen()
{
	const float kDistancePlayerTo3DReticle = 50.0f;
	offset = { 0.0,0.0,1.0f };
	offset = XMVector3TransformNormal(offset, cmat);
	offset = XMVector3Normalize(offset) * kDistancePlayerTo3DReticle;
	center->SetRotation(Rot);
	cmat = center->GetMatrix();
	alll = { 0.0f,0.0f,0.0f };
	alll = XMVector3TransformNormal(allPos, cmat);
	{
		XMVECTOR positionRet = alll;

		ChangeViewPort(matViewPort);

		XMMATRIX matVP = matViewPort;

		XMMATRIX View = camera->GetViewMatrix();
		XMMATRIX Pro = camera->GetProjectionMatrix();

		XMMATRIX matViewProjectionViewport = View * Pro * matVP;

		positionRet = XMVector3TransformCoord(positionRet, matViewProjectionViewport);

		RockOn->SetPosition({ positionRet.m128_f32[0],positionRet.m128_f32[1] });


		Ene2DPos.x = positionRet.m128_f32[0];
		Ene2DPos.y = positionRet.m128_f32[1];
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
