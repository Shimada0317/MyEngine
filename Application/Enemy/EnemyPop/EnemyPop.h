#pragma once
#include"Camera.h"
#include"HelperMath.h"
#include"NormalEnemy.h"
#include"ThrowEnemy.h"
#include"BossEnemy.h"
#include"DirectXCommon.h"
#include<sstream>

class EnemyPop
{
private:
	enum ENEMYPATERN {
		kNormal = 0,
		kThrow,
		kBoss,
	};
public:
	/// <summary>
	/// csv読み込み
	/// </summary>
	void LoadCsv();
	void PopEnemy(int phase,Camera* camera);
	/// <summary>
	/// 敵の更新
	/// </summary>
	/// <param name="player2Dpos"></param>
	/// <param name="playerhp"></param>
	/// <param name="playerbulletshot"></param>
	void Update(const XMFLOAT2& player2Dpos, int& playerhp, bool& playerbulletshot);
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
	list<unique_ptr<NormalEnemy>>robot_;
	list<unique_ptr<BossEnemy>>boss_;
	list<unique_ptr<ThrowEnemy>>throw_;
	bool otherenemyarive_ = true;
};

