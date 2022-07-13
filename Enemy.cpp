#include "Enemy.h"
#include<cassert>

void Enemy::Initalize()
{
	input = Input::GetInstance();
	debugtext = DebugText::GetInstance();
	model = ObjModel::LoadFromObJ("block0");
	enemy= Object3d::Create();
	enemy->SetModel(model);

	position = enemy->GetPosition();
}

void Enemy::Set()
{
	enemy->SetPosition(position);

	
}

void Enemy::Update()
{
	Set();
	enemy->Update();
}

void Enemy::Draw(ID3D12GraphicsCommandList* cmdList)
{
	enemy->Draw();
}

void Enemy::Finalize()
{
}
