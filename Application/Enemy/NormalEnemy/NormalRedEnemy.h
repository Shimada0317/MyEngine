#pragma once
#include"BaseEnemy.h"
class NormalRedEnemy :
    public BaseEnemy
{
private:
    //状態遷移の関数ポインタ
    static void (NormalRedEnemy::* StateFuncTable[])();
public:
    NormalRedEnemy(const XMFLOAT3& allrot, const XMVECTOR& allpos, const XMVECTOR& trackpoint);

    void Activity()override;
private:
	/// <summary>
	/// 変形
	/// </summary>
	void Defomation();
	/// <summary>
	/// プレイヤー追尾モード
	/// </summary>
	void TrackMode();
	/// <summary>
	/// 到着後の待機
	/// </summary>
	void WaitMode();
	/// <summary>
	/// 待機後の攻撃
	/// </summary>
	void AttackMode();
	/// <summary>
	/// 死んだ
	/// </summary>
	void Death();
};

