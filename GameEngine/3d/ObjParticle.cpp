#include "ObjParticle.h"
#include<time.h>

ObjParticle::~ObjParticle()
{

}

void ObjParticle::InitializeState(int i)
{
	Gear_Pos[i] = { 0.0f,0.0f,0.0f };
	Gear_Pos[i].m128_f32[0] = 0.1f * i;
	Gear_Scl[i] = { 0.2f,0.2f,0.2f };
	Worm_Pos[i] = { 0.0f,0.0f,0.0f };
	Worm_Scl[i] = { 0.3f,0.3f,0.3f };
	Effect_F[i] = false;
	Gravity[i] = 0.4f;
}

void ObjParticle::Initialize()
{
	for (int i = 0; i < MAX; i++) {
		Worm[i] = Object3d::Create(ModelManager::GetInstance()->GetModel(0));
		Gear[i] = Object3d::Create(ModelManager::GetInstance()->GetModel(1));

		InitializeState(i);
		Gear[i]->SetPosition(Gear_Pos[i]);
		Gear[i]->SetScale(Gear_Scl[i]);
		Gear[i]->SetRotation(Gear_Rot);
		Worm[i]->SetPosition(Worm_Pos[i]);
		Worm[i]->SetScale(Worm_Scl[i]);
		Worm[i]->SetRotation(Worm_Rot);
		Up[i] = { 0.01f,0.01f,0.005f };
	}
}

void ObjParticle::Set(XMVECTOR& enemyPos, XMFLOAT3& allRot)
{
	for (int i = 0; i < MAX; i++) {
		if (Effect_F[i] == false) {
			InitializeState(i);
			Gear_Pos[i] = enemyPos;
			Worm_Pos[i] = enemyPos;
			Gear_Rot = allRot;
			int gear_radX = (rand() % 10);
			int gear_radY = (rand() % 3 + 1);
			int gear_radZ = (rand() % 10);
			int worm_radX = (rand() % 5);
			int worm_radY = (rand() % 3 + 1);
			int worm_radZ = (rand() % 5);
			if (gear_radX == 2) {
				gear_radX = 1;
				worm_radX = -1;
			}
			else if (gear_radX == 1) {
				gear_radX = -1;
				worm_radX = 1;
			}
			else {
				gear_radX = 0;
				worm_radX = 0;
			}

			if (gear_radZ == 2) {
				gear_radZ = 1;
				worm_radZ = -1;
			}
			else if (gear_radZ == 1) {
				gear_radZ = -1;
				worm_radZ = 1;
			}
			else {
				gear_radZ = 0;
				worm_radZ = 0;
			}
			float gear_posX = 0;
			float gear_posY = 0;
			float gear_posZ = 0;

			float worm_posX = 0;
			float worm_posY = 0;
			float worm_posZ = 0;

			int rad = (rand() % 10) + 5;

			gear_posX = (float)gear_radX / rad;
			gear_posY = (float)gear_radY / 10;
			gear_posZ = (float)gear_radZ / rad;
			worm_posX = (float)worm_radX / rad;
			worm_posY = (float)worm_radY / 10;
			worm_posZ = (float)worm_radZ / rad;
			Gear_NumX[i] = gear_posX;
			Gear_NumY[i] = gear_posY;
			Gear_NumZ[i] = gear_posZ;
			Worm_NumX[i] = worm_posX;
			Worm_NumY[i] = worm_posY;
			Worm_NumZ[i] = worm_posZ;
		}

		Gear[i]->SetPosition(Gear_Pos[i]);
		Gear[i]->SetScale(Gear_Scl[i]);
		Gear[i]->SetRotation(Gear_Rot);
		Worm[i]->SetPosition(Worm_Pos[i]);
		Worm[i]->SetScale(Gear_Scl[i]);
		Worm[i]->SetRotation(Gear_Rot);
	}
}

void ObjParticle::Updata(XMVECTOR& enemyPos,XMFLOAT3& allRot)
{
	Set(enemyPos,allRot);
	Effect();

	for (int i = 0; i < MAX; i++) {
		Gear_Rot.x += 0.5f / 20;
		if (Gear_Pos[i].m128_f32[1] >= enemyPos.m128_f32[1] - 0.1f) {
			Gear_Pos[i].m128_f32[0] += Gear_NumX[i] / 20;
			Gear_Pos[i].m128_f32[1] += Gravity[i] / 15;
			Gear_Pos[i].m128_f32[2] += Gear_NumZ[i] / 20;
		}
		if (Worm_Pos[i].m128_f32[1] >= enemyPos.m128_f32[1] - 0.1f) {
			Worm_Pos[i].m128_f32[0] += Worm_NumX[i] / 20;
			Worm_Pos[i].m128_f32[1] += Gravity[i] / 10;
			Worm_Pos[i].m128_f32[2] += Worm_NumX[i] / 20;
		}

		Gravity[i] -= 0.002;

		Gear_Scl[i].x -= 0.01f/30;
		Gear_Scl[i].y -= 0.01f/30;
		Gear_Scl[i].z -= 0.01f/30;
		if (Gear_Scl[i].x <= 0 && Gear_Scl[i].y <= 0 && Gear_Scl[i].z <= 0) {
			Gear_Scl[i].x = 0.0f;
			Gear_Scl[i].y = 0.0f;
			Gear_Scl[i].z = 0.0f;
			Delete_ = true;
			break;
		}
		Gear[i]->Updata();
		Worm[i]->Updata();
	}
}

void ObjParticle::Effect()
{
	for (int i = 0; i < MAX; i++) {
		Effect_F[i] = true;
	}
}

void ObjParticle::Draw()
{
	for (int i = 0; i < MAX; i++) {
		Gear[i]->Draw();
		Worm[i]->Draw();
	}
}

void ObjParticle::Finalize()
{
	for (int i = 0; i < MAX; i++) {
		Gear[i].reset();
	}
}
