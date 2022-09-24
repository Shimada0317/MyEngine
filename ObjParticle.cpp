#include "ObjParticle.h"

void ObjParticle::Initialize()
{
	model = ObjModel::CreateFromOBJ("particle");
	for (int i = 0; i < MAX; i++) {
		particle[i] = Object3d::Create();
		particle[i]->SetModel(model);
		position[i] = { 0.0f,0.0f,0.0f };
		scale[i] = { 0.3f,0.3f,0.3f };
		particle[i]->SetPosition(position[i]);
		particle[i]->SetScale(scale[i]);
		particle[i]->SetRotation(rotation);
		effect[i] = true;
		up[i] = { 0.005f,0.005f,0.005f };
	}
}

void ObjParticle::Set(XMVECTOR& enemyPos,bool arive)
{
	for (int i = 0; i < MAX; i++) {
		if (arive == true) {
			position[i] = enemyPos;
		}
		particle[i]->SetPosition(position[i]);
		particle[i]->SetScale(scale[i]);
		particle[i]->SetRotation(rotation);
	}
}

void ObjParticle::Update()
{
	
	for (int i = 0; i < MAX; i++) {
		if (scale[i].x >= 0.0f && scale[i].y >= 0.0f && scale[i].z >= 0.0f) {
			scale[i].x -= up[i].x;
			scale[i].y -= up[i].y;
			scale[i].z -= up[i].z;
			position[i].m128_f32[1] += 0.01f;
		}
		else {
			effect[i] = false;
		}
		particle[i]->Update();
	}
	//Set();
}

void ObjParticle::Draw()
{
	for (int i = 0; i < MAX; i++) {
		if (effect[i] == true) {
			particle[i]->Draw();
		}
	}
}

void ObjParticle::Finalize()
{
	for (int i = 0; i < MAX; i++) {
		particle[i].reset();
	}
}
