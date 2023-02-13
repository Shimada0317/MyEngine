#include "ObjParticle.h"
#include<time.h>
#include"Action.h"

using namespace DirectX;

ObjParticle::~ObjParticle()
{

}

const XMFLOAT3 operator-(const DirectX::XMFLOAT3& lhs, const DirectX::XMFLOAT3& rhs)
{
	XMFLOAT3 result;
	result.x = lhs.x - rhs.x;
	result.y = lhs.y - rhs.y;
	result.z = lhs.z - rhs.z;
	return result;
}

const XMFLOAT3 operator/(const DirectX::XMFLOAT3& lhs, const float rhs)
{
	XMFLOAT3 result;
	result.x = lhs.x / rhs;
	result.y = lhs.y / rhs;
	result.z = lhs.z / rhs;
	return result;
}

void ObjParticle::Initialize(int ModelNumber, const XMVECTOR& particlepos, const XMFLOAT3& particlescl, const XMFLOAT3& particlerot)
{
	Particle = Object3d::Create(ModelManager::GetInstance()->GetModel(ModelNumber));
	RandomZ = Action::GetInstance()->GetRangRand(-0.01f, 0.01);
	RandomY= Action::GetInstance()->GetRangRand(0.01f, 0.03f);
	RandomX = Action::GetInstance()->GetRangRand(-0.01f, 0.01f);
	SmoleScl = Action::GetInstance()->GetRangRand(0.05f, 0.07f);
	ParticlePos = particlepos;
	ParticleScl = particlescl;
	ParticleRot = particlerot;
}

void ObjParticle::Set()
{
	Particle->SetPosition(ParticlePos);
	Particle->SetRotation(ParticleRot);
	Particle->SetScale(ParticleScl);
}

void ObjParticle::Update(XMVECTOR& enemyPos,XMFLOAT3& allRot)
{
	Set();
	XMFLOAT3 ConvertValue = Action::GetInstance()->ConvertToXMFLOAT3(SmoleScl);
	ConvertValue = ConvertValue / 100;
	time += 0.001f;
	ParticlePos.m128_f32[2] += RandomZ;
	ParticlePos.m128_f32[1] += RandomY-gravity*time;
	ParticlePos.m128_f32[0] += RandomX;
	ParticleRot.x += RandomX;
	ParticleRot.y += RandomY;
	ParticleScl = ParticleScl - ConvertValue;
	if (ParticleScl.x <= 0 && ParticleScl.y <= 0 && ParticleScl.z <= 0) {
		Delete_ = true;
	}
	Particle->Update();
}

void ObjParticle::Draw()
{
	Particle->Draw();
}

void ObjParticle::Finalize()
{
	Particle.reset();
}
