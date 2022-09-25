#include "ObjParticle.h"
#include<time.h>

void ObjParticle::InitializeState(int i)
{
	//for (int i = 0; i < MAX; i++) {
	position[i] = { 0.0f,0.0f,0.0f };
	position[i].m128_f32[0] = 0.1f * i;
	scale[i] = { 0.0f,0.0f,0.3f };
	scalenumX = rand() % 3 + 1;
	scalenumY = rand() % 3 + 1;
	scalenumX = scalenumX / 10;
	scalenumY = scalenumY / 10;
	scale[i].x = scale[i].x + scalenumX;
	scale[i].y = scale[i].y + scalenumY;
	effect[i] = true;
	//}
}

void ObjParticle::Initialize()
{
	model = ObjModel::CreateFromOBJ("particle");
	for (int i = 0; i < MAX; i++) {
		particle[i] = Object3d::Create();
		particle[i]->SetModel(model);
		InitializeState(i);
		particle[i]->SetPosition(position[i]);
		particle[i]->SetScale(scale[i]);
		particle[i]->SetRotation(rotation);
		up[i] = { 0.05f,0.05f,0.005f };
	}
}

void ObjParticle::Set(XMVECTOR& enemyPos, bool arive)
{
	srand((unsigned)time(NULL));
	
	for (int i = 0; i < MAX; i++) {
		if (arive == true) {
			InitializeState(i);
			position[i] = enemyPos;
			numX = rand() % 6 - 6;
			numY = rand() % 6 - 3;
			numX = numX / 10;
			numY = numY / 10;
			position[i].m128_f32[0] = position[i].m128_f32[0] + numX;
			position[i].m128_f32[1] = position[i].m128_f32[1] + numY;
		}
		particle[i]->SetPosition(position[i]);
		particle[i]->SetScale(scale[i]);
		particle[i]->SetRotation(rotation);
	}
}

void ObjParticle::Update()
{

	for (int i = 0; i < MAX; i++) {
		if (scale[i].x >= 0.0f && scale[i].y >= 0.0f ) {
			scale[i].x -= up[i].x;
			scale[i].y -= up[i].y;
			//scale[i].z -= up[i].z;
			//position[i].m128_f32[0] = position[i].m128_f32[0] - 0.1f;
			//position[i].m128_f32[1] = position[i].m128_f32[1] - 0.1f;
			//position[i].m128_f32[0] += 0.01f * i;
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
