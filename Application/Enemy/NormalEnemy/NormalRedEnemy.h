#pragma once
#include"BaseEnemy.h"
class NormalRedEnemy :
    public BaseEnemy
{
private:
    //��ԑJ�ڂ̊֐��|�C���^
    static void (NormalRedEnemy::* StateFuncTable[])();
public:
    NormalRedEnemy(const XMFLOAT3& allrot, const XMVECTOR& allpos, const XMVECTOR& trackpoint);

    void Activity()override;
private:
	/// <summary>
	/// �ό`
	/// </summary>
	void Defomation();
	/// <summary>
	/// �v���C���[�ǔ����[�h
	/// </summary>
	void TrackMode();
	/// <summary>
	/// ������̑ҋ@
	/// </summary>
	void WaitMode();
	/// <summary>
	/// �ҋ@��̍U��
	/// </summary>
	void AttackMode();
	/// <summary>
	/// ����
	/// </summary>
	void Death();
};

