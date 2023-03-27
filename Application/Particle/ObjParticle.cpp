#include "ObjParticle.h"
#include<time.h>
#include"Action.h"
#include"EasyMath.h"

using namespace DirectX;

ObjParticle::~ObjParticle()
{
	Particle.reset();
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
	RandomZ = Action::GetInstance()->GetRangRand(-0.1f, 0.1);
	RandomY= Action::GetInstance()->GetRangRand(0.1f, 0.3f);
	RandomX = Action::GetInstance()->GetRangRand(-0.05f, 0.05f);
	SmoleScl = Action::GetInstance()->GetRangRand(0.5f, 0.7f);
	RandomRot = Action::GetInstance()->GetRangRand(5.0f, 10.0f);
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

void ObjParticle::Update()
{
	Set();
	XMFLOAT3 ConvertValue = EasyMath::GetInstance()->ConvertToXMFLOAT3(SmoleScl);
	ConvertValue = ConvertValue / 200;
	time += 0.005;
	ParticlePos.m128_f32[2] += RandomZ;
	ParticlePos.m128_f32[1] += RandomY-gravity*time;
	ParticlePos.m128_f32[0] += RandomX;
	ParticleRot.x += RandomRot;
	ParticleRot.y += RandomRot;
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
