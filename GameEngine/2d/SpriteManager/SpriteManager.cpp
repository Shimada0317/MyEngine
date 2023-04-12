#include "SpriteManager.h"

void SpriteManager::SpriteName()
{
#pragma region タイトルからあるもの
	Sprite::LoadTexture(kTitle, L"Resources/Title.png");
	Sprite::LoadTexture(kReticle, L"REsources/Reticle.png");
	Sprite::LoadTexture(kClickStartBefore, L"Resources/ClickStartBefore.png");
	Sprite::LoadTexture(kClickStartAfter, L"Resources/ClickStartAfter.png");
	Sprite::LoadTexture(kSignalBefore, L"Resources/SIGNALBefore.png");
	Sprite::LoadTexture(kSignalAfter, L"Resources/SIGNALAfter.png");
	Sprite::LoadTexture(kOperationDescription, L"Resources/OperationDescription.png");
	Sprite::LoadTexture(kEnemyDescription, L"Resources/EnemyDescription.png");
	Sprite::LoadTexture(kStartScreen, L"Resources/StartScreen.png");
	Sprite::LoadTexture(kArrowRight, L"Resources/ArrowRight.png");
	Sprite::LoadTexture(kArrowLeft, L"Resources/ArrowLeft.png");
#pragma endregion

#pragma region プレイヤーのステータスで必要な物
	Sprite::LoadTexture(kHart, L"Resources/Hart.png");
	Sprite::LoadTexture(kOne, L"Resources/One.png");
	Sprite::LoadTexture(kTwo, L"Resources/Two.png");
	Sprite::LoadTexture(kThree, L"Resources/Three.png");
	Sprite::LoadTexture(kFour, L"Resources/Four.png");
	Sprite::LoadTexture(kFive, L"Resources/Five.png");
	Sprite::LoadTexture(kBullet, L"Resources/Bullet.png");
	Sprite::LoadTexture(kReload, L"Resources/Reload.png");
	Sprite::LoadTexture(kShot, L"Resources/Shot.png");
#pragma endregion

#pragma region 敵で使われている
	Sprite::LoadTexture(kEnemyMarker, L"Resources/EnemyMarker.png");
#pragma endregion

#pragma region その他ゲームで使われているもの
	Sprite::LoadTexture(kWindow, L"Resources/Window.png");
	Sprite::LoadTexture(kDamageEffect, L"Resources/DamageEfect.png");
	Sprite::LoadTexture(kCurtain, L"Resources/Curtain.png");
	Sprite::LoadTexture(kSkip, L"Resources/Skip.png");
	Sprite::LoadTexture(kGameClear, L"Resources/GameClear.png");
	Sprite::LoadTexture(kGameOver, L"Resources/GameOver.png");
	Sprite::LoadTexture(kYes, L"Resources/Yes.png");
	Sprite::LoadTexture(kNo, L"Resources/No.png");
#pragma endregion



}
