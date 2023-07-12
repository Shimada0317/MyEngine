#pragma once
#include"Camera.h"
#include"HelperMath.h"
#include"BossEnemy.h"
#include"BaseEnemy.h"
#include"BaseThrow.h"
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
	/// csv�ǂݍ���
	/// </summary>
	void LoadCsv();
	/// <summary>
	/// �G�̐���
	/// </summary>
	/// <param name="phase"></param>
	/// <param name="camera"></param>
	void PopEnemy(int phase,Camera* camera);
	/// <summary>
	/// �G�̍X�V
	/// </summary>
	/// <param name="player2Dpos"></param>
	/// <param name="playerhp"></param>
	/// <param name="playerbulletshot"></param>
	void Update(Player* player);
	/// <summary>
	/// �G���m�̒ǔ��悪��������̊m�F�Ƃ��̏ꍇ�̏���
	/// </summary>
	void CheckSameTrackPosition();
	/// <summary>
	/// �G�̎��S����
	/// </summary>
	void EnemyDead();
	/// <summary>
	/// �S�Ă̓G��|������
	/// </summary>
	/// <returns></returns>
	bool KilledAllEnemy();
	/// <summary>
	/// �`�揈��
	/// </summary>
	/// <param name="dxcommon"></param>
	void Draw(DirectXCommon* dxcommon);
private:
	stringstream enemypopcommands_;
	list<unique_ptr<BossEnemy>>boss_;
	list<unique_ptr<BaseEnemy>>base_type_;
	bool otherenemyarive_ = true;
};

