#pragma once
#include "BaseThrow.h"
class ThrowBlueEnemy :
    public BaseThrow
{
private:
	//��ԑJ�ڂ̊֐��|�C���^
	static void (ThrowBlueEnemy::* StateFuncTable[])();
public:
	ThrowBlueEnemy(const XMFLOAT3& allrot, const XMVECTOR& allpos, const XMVECTOR& trackpos);

	void Activity()override;
public:
	/// <summary>
	/// �o�ꏈ��
	/// </summary>
	void AppearanceProcess();
	/// <summary>
	/// �ҋ@����
	/// </summary>
	void WaitProcess();
	/// <summary>
	///	�U������
	/// </summary>
	void AttackProcess();
	/// <summary>
	/// ���S����
	/// </summary>
	void DeathProcess();


};

