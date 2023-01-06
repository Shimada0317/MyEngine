#include "Head.h"

Head::~Head()
{
	delete headModel;
}

void Head::Initialize(bool& arive, const XMVECTOR& bodyPos, const XMFLOAT3 rotation, Camera* camera)
{
	headModel = ObjModel::CreateFromOBJ("Head");
	Head = Object3d::Create(headModel);

	
	HeadPos = bodyPos;
	HeadRot = rotation;
	HeadPos.m128_f32[1] = bodyPos.m128_f32[1] +1.0f;
	Head->SetPosition(HeadPos);
	Head->SetRotation(HeadRot);
	Head->SetScale(HeadScl);
	damageEfect = ParticleManager::Create(camera);

	MotionRad = (rand() % 2);
}

void Head::SetPRS(const XMVECTOR& bodyPos, const XMFLOAT3 rotation, Bullet* bull)
{

	bullPos = bull->GetPosition();
	bullScl = bull->GetScl();
	Hit = bull->GetTrigger();
	

	HeadPos = bodyPos;

	HeadPos.m128_f32[1] = bodyPos.m128_f32[1] +1.0f;
	Head->SetPosition(HeadPos);

	Head->SetScale(HeadScl);
}

void Head::Updata(bool& arive,const XMVECTOR& bodyPos, const XMFLOAT3 rotation, Bullet* bull,int& Hp)
{
	if (arive == true) {
		//SetPRS(bodyPos);
		
		SetPRS(bodyPos, rotation, bull);
		if (Collision::HeadHit(HeadPos, HeadScl, bullPos, bullScl)) {
			Hp -= 50;
			//XMVECTOR pos = { 0.0f,1000.0f,0.0f };
			Hit = false;
			particle = true;
			bull->SetTrigger(Hit);
			//bull->SetPosition(pos);
			HitColor = { 1,0,0,1 };
		}
		else {
			HitColor = { 1,1,1,1 };
		}
		SetPRS(bodyPos, rotation, bull);
		ParticleEfect();
	}
	else {
		HitColor.w -= 0.01f;
	}
	
	damageEfect->Update({ 0.0f,0.0f,0.0f,1.0f });
	Head->Updata(HitColor);
}

void Head::Draw(bool arive)
{
	if (HitColor.w >= 0) {
		Head->Draw();
	}
}

void Head::Motion(const float& rot)
{

	if (MotionRad == 0) {
		if (radChange == true) {
			HeadRot.x += rot;
			//HeadRot.x += rot;
		}
		else {
			HeadRot.x -= rot;
			//HeadRot.z -= rot;
		}
		if (HeadRot.x >= 10) {
			radChange = false;
		}
		else if (HeadRot.x <= -10) {
			radChange = true;
		}
	}

	if (MotionRad == 1) {
		if (radChange == true) {
			//HeadRot.x += rot;
			HeadRot.y += rot;
		}
		else {
			//HeadRot.x -= rot;
			HeadRot.y -= rot;
		}
		if (HeadRot.y >= 10) {
			radChange = false;
		}
		else if (HeadRot.y <= -10) {
			radChange = true;
		}
	}


	Head->SetRotation(HeadRot);
}

void Head::ParticleDraw(ID3D12GraphicsCommandList* cmdeList)
{
	ParticleManager::PreDraw(cmdeList);
	damageEfect->Draw();
	ParticleManager::PostDraw();
}

void Head::ParticleEfect()
{
	if (particle == true) {
		for (int i = 0; i < 10; i++) {
			const float rnd_pos = 0.0f;
			XMFLOAT3 pos;
			XMFLOAT3 po2;
			
			pos.x = HeadPos.m128_f32[0];
			pos.y = HeadPos.m128_f32[1];
			pos.z = HeadPos.m128_f32[2];
			
			const float rnd_vel = 0.001f;
			XMFLOAT3 vel{};
			vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
			vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
			vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

			XMFLOAT3 acc{};
			XMFLOAT3 ac2{};
			acc.y = -0.00001;
			acc.y = -0.00001;

			damageEfect->Add(10, pos, vel, acc, 2.5f, 0.2f,0);
		}
		particle = false;
	}
}

void Head::Finalize()
{
	Head.reset();
	delete headModel;
}
