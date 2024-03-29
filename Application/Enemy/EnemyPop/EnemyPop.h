#pragma once
#include"Camera.h"
#include"HelperMath.h"
#include"BossEnemy.h"
#include"BaseEnemy.h"
#include"RocketEnemy.h"
#include"DirectXCommon.h"
#include"BaseEnemy.h"
#include<sstream>

class Player;

class EnemyPop
{
private:
	enum ENEMYPATERN {
		kNormal = 0,
		kThrow,
		kThrowRed,
		kThrowGreen,
		kThrowBlue,
		kBoss,
		kLow,
		kRocket,
		kNormalRed,
	};
public:
	/// <summary>
	/// csv読み込み
	/// </summary>
	void LoadCsv();
	/// <summary>
	/// 敵の生成
	/// </summary>
	/// <param name="phase"></param>
	/// <param name="camera"></param>
	void PopEnemy(int phase,Camera* camera);
	/// <summary>
	/// 敵の更新
	/// </summary>
	/// <param name="player2Dpos"></param>
	/// <param name="playerhp"></param>
	/// <param name="playerbulletshot"></param>
	void Update(Player* player);
	/// <summary>
	/// 敵同士の追尾先が被った時の確認とその場合の処理
	/// </summary>
	void CheckSameTrackPosition();
	/// <summary>
	/// 敵の死亡処理
	/// </summary>
	void EnemyDead();
	/// <summary>
	/// 全ての敵を倒した時
	/// </summary>
	/// <returns></returns>
	bool KilledAllEnemy();
	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="dxcommon"></param>
	void Draw(DirectXCommon* dxcommon);
private:
	stringstream enemypopcommands_;
	list<unique_ptr<BossEnemy>>boss_;
	list<unique_ptr<BaseEnemy>>base_type_;
	bool otherenemyarive_ = true;
};

