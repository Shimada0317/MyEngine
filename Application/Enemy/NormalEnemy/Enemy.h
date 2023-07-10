#pragma once
#include "Base.h"
class Enemy :
    public Base
{
private:
    //状態遷移の関数ポインタ
    static void (Enemy::* StateFuncTable[])();
public:
    Enemy(const XMFLOAT3& allrot, const XMVECTOR& allpos, const XMVECTOR& trackpos);

    //void Activity()override;
public:
	/// <summary>
	/// 登場処理
	/// </summary>
	void AppearanceProcess();
	/// <summary>
	/// 待機処理
	/// </summary>
	void WaitProcess();
	/// <summary>
	///	攻撃処理
	/// </summary>
	void AttackProcess();
	/// <summary>
	/// 死亡処理
	/// </summary>
	void DeathProcess();
};

