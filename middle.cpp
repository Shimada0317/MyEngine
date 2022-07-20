#include "middle.h"

void middle::Initialize()
{
	player = new Player();
	player->Initalize();

	enemy = new Enemy();
	enemy->Initalize();

	bull = new Bullet();
	bull->Initialize();
}

void middle::SetPSR()
{
	playerPos=player->GetPosition();


}

void middle::AllUpdate()
{
}

void middle::Update()
{
}

void middle::Draw()
{
}

void middle::Fainalize()
{
}
