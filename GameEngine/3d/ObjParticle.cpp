#include "ObjParticle.h"
#include<time.h>

ObjParticle::~ObjParticle()
{
	//delete model;
	//delete worm;
}

void ObjParticle::Fast()
{
}

void ObjParticle::InitializeState(int i)
{
	//for (int i = 0; i < MAX; i++) {
	position[i] = { 0.0f,0.0f,0.0f };
	position[i].m128_f32[0] = 0.1f * i;
	scale[i] = { 0.3f,0.3f,0.3f };
	Wposition[i] = { 0.0f,0.0f,0.0f };
	wscale[i] = { 0.3f,0.3f,0.3f };
	effect[i] = false;
	//numY[i] = rand() % 1 + 0.2f;
	//}
}

void ObjParticle::Initialize()
{
	//worm = ObjModel::CreateFromOBJ("Worm");
	//model = ObjModel::CreateFromOBJ("Gear");
	for (int i = 0; i < MAX; i++) {
		Worm[i] = Object3d::Create(ModelManager::GetInstance()->GetModel(0));
		particle[i] = Object3d::Create(ModelManager::GetInstance()->GetModel(1));

		InitializeState(i);
		particle[i]->SetPosition(position[i]);
		particle[i]->SetScale(scale[i]);
		particle[i]->SetRotation(rotation);
		Worm[i]->SetPosition(Wposition[i]);
		Worm[i]->SetScale(wscale[i]);
		Worm[i]->SetRotation(Wrotation);
		up[i] = { 0.01f,0.01f,0.005f };
	}
}

void ObjParticle::Set(XMVECTOR& enemyPos)
{
	for (int i = 0; i < MAX; i++) {
		if (effect[i] == false) {
			InitializeState(i);
			position[i] = enemyPos;
			Wposition[i] = enemyPos;
			int radX = (rand() % 10);
			int radY = (rand() % 3+1);
			int womX = (rand() % 5);
			int womY = (rand() % 3 + 1);
			if (radX == 2) {
				radX = 1;
				womX = -1;
			}
			else if(radX==1) {
				radX = -1;
				womX = 1;
			}
			else {
				radX = 0;
				womX = 0;
			}
			float poiX = 0;
			float poiY = 0;

			float wpoiX = 0;
			float wpoiY = 0;

			int rad = (rand() % 10)+5;

			poiX = (float)radX / rad;
			poiY = (float)radY / 10;
			wpoiX = (float)womX / rad;
			wpoiY = (float)womY / 10;
			numX[i] = poiX;
			numY[i] = poiY;
			wnumX[i] = wpoiX;
			wnumY[i] = wpoiY;
		}
		
		particle[i]->SetPosition(position[i]);
		particle[i]->SetScale(scale[i]);
		particle[i]->SetRotation(rotation);
		Worm[i]->SetPosition(Wposition[i]);
		Worm[i]->SetScale(scale[i]);
		Worm[i]->SetRotation(rotation);
	}
}

void ObjParticle::Updata(XMVECTOR& enemyPos)
{
	Set(enemyPos);
	Effect();

	for (int i = 0; i < MAX; i++) {
			rotation.x += 0.5f/20;
			position[i].m128_f32[1] +=numY[i]/20;
			position[i].m128_f32[0] += numX[i]/20;
			Wposition[i].m128_f32[0] += wnumX[i]/20;
			Wposition[i].m128_f32[1] += wnumY[i]/20;
			scale[i].x -= 0.01f/20;
			scale[i].y -= 0.01f/20;
			scale[i].z -= 0.01f/20;
			if (scale[i].x <=0 && scale[i].y <= 0 && scale[i].z <= 0) {
				scale[i].x = 0.0f;
				scale[i].y = 0.0f;
				scale[i].z = 0.0f;
				break;
			}
		particle[i]->Updata();
		Worm[i]->Updata();
	}
	if (--deleteTime_ <= 0) {
		delete_ = true;
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

			particle[i]->Draw();
			Worm[i]->Draw();
		
	}
}

void ObjParticle::Finalize()
{
	for (int i = 0; i < MAX; i++) {
		particle[i].reset();
	}
	delete model;
	delete worm;
}
