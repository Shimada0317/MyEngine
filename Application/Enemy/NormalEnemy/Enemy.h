#pragma once
#include "Base.h"
class Enemy :
    public Base
{
private:
    //ó‘Ô‘JˆÚ‚ÌŠÖ”ƒ|ƒCƒ“ƒ^
    static void (Enemy::* StateFuncTable[])();
public:
    Enemy(const XMFLOAT3& allrot, const XMVECTOR& allpos, const XMVECTOR& trackpos);

    //void Activity()override;
public:
	/// <summary>
	/// “oêˆ—
	/// </summary>
	void AppearanceProcess();
	/// <summary>
	/// ‘Ò‹@ˆ—
	/// </summary>
	void WaitProcess();
	/// <summary>
	///	UŒ‚ˆ—
	/// </summary>
	void AttackProcess();
	/// <summary>
	/// €–Sˆ—
	/// </summary>
	void DeathProcess();
};

