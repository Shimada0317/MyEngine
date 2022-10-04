#include "ObjParticle.h"
#include<time.h>

void ObjParticle::InitializeState(int i)
{
	//for (int i = 0; i < MAX; i++) {
	position[i] = { 0.0f,0.0f,0.0f };
	position[i].m128_f32[0] = 0.1f * i;
	scale[i] = { 0.3f,0.3f,0.3f };
	effect[i] = false;
	//numY[i] = rand() % 1 + 0.2f;
	//}
}

void ObjParticle::Initialize()
{
	model = ObjModel::CreateFromOBJ("Gear");
	for (int i = 0; i < MAX; i++) {

		particle[i] = Object3d::Create();
		particle[i]->SetModel(model);
		InitializeState(i);
		particle[i]->SetPosition(position[i]);
		particle[i]->SetScale(scale[i]);
		particle[i]->SetRotation(rotation);
		up[i] = { 0.01f,0.01f,0.005f };
	}
}

void ObjParticle::Set(XMVECTOR& enemyPos)
{
	for (int i = 0; i < MAX; i++) {
		if (effect[i] == false) {
			InitializeState(i);
			position[i] = enemyPos;
			int radX = (rand() % 10);
			int radY = (rand() % 3+1);
			if (radX == 2) {
				radX = 1;
			}
			else if(radX==1) {
				radX = -1;
			}
			else {
				radX = 0;
			}
			float poiX = 0;
			float poiY = 0;
			int rad = (rand() % 10)+5;

			poiX = (float)radX / rad;
			poiY = (float)radY / 10;
			numX[i] = poiX;
			numY[i] = poiY;
		}
		
		particle[i]->SetPosition(position[i]);
		particle[i]->SetScale(scale[i]);
		particle[i]->SetRotation(rotation);
	}
}

void ObjParticle::Update()
{

	for (int i = 0; i < MAX; i++) {
		
		if (effect[i] == true) {
			rotation.x += 0.5f;
			position[i].m128_f32[1] +=numY[i];
			position[i].m128_f32[0] += numX[i];
			scale[i].x -= 0.01f;
			scale[i].y -= 0.01f;
			scale[i].z -= 0.01f;
			if (scale[i].x <=0 && scale[i].y <= 0 && scale[i].z <= 0) {
				scale[i].x = 0.0f;
				scale[i].y = 0.0f;
				scale[i].z = 0.0f;
				effect[i] = false;
			}
		}
		particle[i]->Update();
	}
}

void ObjParticle::Effect()
{
	for (int i = 0; i < MAX; i++) {
		effect[i] = true;
	}
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
	delete model;
}
