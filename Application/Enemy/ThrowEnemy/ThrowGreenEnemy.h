#pragma once
#include "BaseThrow.h"
class ThrowGreenEnemy :
    public BaseThrow
{
private:
	//��ԑJ�ڂ̊֐��|�C���^
	static void (ThrowGreenEnemy::* StateFuncTable[])();
public:
	ThrowGreenEnemy(const XMFLOAT3& allrot, const XMVECTOR& allpos, const XMVECTOR& trackpos);

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

