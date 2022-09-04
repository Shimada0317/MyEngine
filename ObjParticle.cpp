#include "ObjParticle.h"

void ObjParticle::Initialize()
{
	model = ObjModel::CreateFromOBJ("particle");
	for (int i = 0; i < 10; i++) {
		particle[i] = Object3d::Create();
		particle[i]->SetModel(model);
		particle[i]->SetPosition(position);
		particle[i]->SetScale(scale);
		particle[i]->SetRotation(rotation);
	}
}

void ObjParticle::Set(XMVECTOR& enemyPos)
{
	position = enemyPos;
	for (int i = 0; i < 10; i++) {
		particle[i]->SetPosition(position);
		particle[i]->SetScale(scale);
		particle[i]->SetRotation(rotation);
	}
}

void ObjParticle::Update()
{

		particle[0]->Update();
	
}

void ObjParticle::Draw()
{
	for (int i = 0; i < 10; i++) {
		particle[i]->Draw();
	}
}

void ObjParticle::Finalize()
{
}
