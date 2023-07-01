#pragma once
#include"Camera.h"
#include"HelperMath.h"
#include"NormalEnemy.h"
#include"LowEnemy.h"
#include"ThrowEnemy.h"
#include"BossEnemy.h"
#include"BaseThrow.h"
#include"RocketEnemy.h"
#include"DirectXCommon.h"
#include<sstream>

class Player;

class EnemyPop
{
private:
	enum ENEMYPATERN {
		kNormal = 0,
		kThrow,
		kBoss,
		kLow,
		kRocket,
	};
public:
	/// <summary>
	/// csv“Ç‚İ‚İ
	/// </summary>
	void LoadCsv();
	/// <summary>
	/// “G‚Ì¶¬
	/// </summary>
	/// <param name="phase"></param>
	/// <param name="camera"></param>
	void PopEnemy(int phase,Camera* camera);
	/// <summary>
	/// “G‚ÌXV
	/// </summary>
	/// <param name="player2Dpos"></param>
	/// <param name="playerhp"></param>
	/// <param name="playerbulletshot"></param>
	void Update(Player* player);
	/// <summary>
	/// “G“¯m‚Ì’Ç”öæ‚ª”í‚Á‚½‚ÌŠm”F‚Æ‚»‚Ìê‡‚Ìˆ—
	/// </summary>
	void CheckSameTrackPosition();
	/// <summary>
	/// “G‚Ì€–Sˆ—
	/// </summary>
	void EnemyDead();
	/// <summary>
	/// ‘S‚Ä‚Ì“G‚ğ“|‚µ‚½
	/// </summary>
	/// <returns></returns>
	bool KilledAllEnemy();
	/// <summary>
	/// •`‰æˆ—
	/// </summary>
	/// <param name="dxcommon"></param>
	void Draw(DirectXCommon* dxcommon);
private:
	stringstream enemypopcommands_;
	list<unique_ptr<NormalEnemy>>robot_;
	list<unique_ptr<BossEnemy>>boss_;
	list<unique_ptr<ThrowEnemy>>throw_;
	list<unique_ptr<LowEnemy>>low_;
	list<unique_ptr<RocketEnemy>>rocket_;
	list<unique_ptr<BaseThrow>>base_;
	bool otherenemyarive_ = true;
};

