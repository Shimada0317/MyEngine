#pragma once
#include"Camera.h"
#include"HelperMath.h"
#include"NormalEnemy.h"
#include"ThrowEnemy.h"
#include"BossEnemy.h"
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
	void LoadCsv();
	void PopEnemy(list<unique_ptr<NormalEnemy>>&normalrobot, list<unique_ptr<ThrowEnemy>>& throwrobot, list<unique_ptr<BossEnemy>>& bossrobot,int phase,Camera* camera);
private:
	stringstream enemypopcommands_;
};

