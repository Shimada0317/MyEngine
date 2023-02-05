#include "ObjParticle.h"
#include<time.h>

ObjParticle::~ObjParticle()
{

}


void ObjParticle::Initialize(int ModelNumber, const XMVECTOR& particlepos, const XMFLOAT3& particlescl, const XMFLOAT3& particlerot)
{
	Particle = Object3d::Create(ModelManager::GetInstance()->GetModel(ModelNumber));

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
