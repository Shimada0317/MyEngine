#pragma once
#include "Base.h"
class Normal :
    public Base
{
private:
	enum State {
		kDefomation,
		kMove,
		kWait,
		kAttack,
		kDeath,
	}state_;

	enum Type {
		kNormal,
		kRapid,
	};
private:
	//��ԑJ�ڂ̊֐��|�C���^
	static void (Normal::* StateFuncTable[])();
public:
    Normal(const XMFLOAT3& allrot, const XMVECTOR& allpos, const XMVECTOR& trackpoint);

    void CreateRobot(Camera* camera)override;

    void StatusSet();

    void AllUpdate();

    void Activity(Player* player)override;

    void Draw(DirectXCommon* dxCommon)override;
private:
	/// <summary>
	/// �ό`
	/// </summary>
	void Defomation();
	/// <summary>
	/// �v���C���[�ǔ����[�h
	/// </summary>
	void TrackPlayerMode();
	/// <summary>
	/// ������̑ҋ@
	/// </summary>
	void WaitMode();
	/// <summary>
	/// �ҋ@��̍U��
	/// </summary>
	void AttackMode();

	void Death();
private:
	/// <summary>
	/// �W�J���̑傫��
	/// </summary>
	void DeploymentScale();
	/// <summary>
	/// �Ǐ]���鎞�A��ɑ��̓G�����邩
	/// </summary>
	/// <param name="otherenemyarive">���M�ȊO����ɂ��邩</param>
	void WaitTrack(bool otherenemyarive);
	/// <summary>
	/// �p�[�c��傫������
	/// </summary>
	void Enlargement();
	/// <summary>
	/// �h�炷����
	/// </summary>
	void ShakeBody();
	/// <summary>
	/// �U���`���[�W
	/// </summary>
	void AttackCharge();
	/// <summary>
	/// 
	/// </summary>
	void AttackBefore();
	/// <summary>
	/// �U��
	/// </summary>
	void Attack();
	/// <summary>
	/// �U�����󂯂��Ƃ��ɐF��ς���
	/// </summary>
	void HitColor();
	/// <summary>
	/// �����ɂ���
	/// </summary>
	void Transparentize();

	void EnemyCollision();
private:
    //Obj
    //��
    std::unique_ptr<Object3d> head_part_;
    //���r
    std::unique_ptr<Object3d> arms_part_;
    //��
    std::unique_ptr<Object3d> body_part_;
	//�p�[�c���Ƃ̃X�P�[��
	XMFLOAT3 headpart_scl_ = { 0.3f,0.3f,0.3f };
	XMFLOAT3 bodypart_scl_ = { 1.0f,1.0f,1.0f };
	XMFLOAT3 armspart_scl_ = { 0.2f,0.2f,0.2f };
	//�p�[�c���Ƃ̃|�W�V����
	XMVECTOR headpart_pos_ = { 0.0f,0.0f,0.0f };
	XMVECTOR bodypart_pos_ = { 0.0f,0.0f,0.0f };
	XMVECTOR armspart_pos_ = { 0.0f,0.0f,0.0f };
	//�p�[�c���Ƃ̐F
	XMFLOAT4 armspart_color_ = { 1.0f,1.0f,1.0f,1.0f };
	XMFLOAT4 headpart_color_ = { 1.0f,1.0f,1.0f,1.0f };
	XMFLOAT4 bodypart_color_ = { 1.0f,1.0f,1.0f,1.0f };
	//�p�[�c���Ƃ̉�]
	XMFLOAT3 armspart_rot_ = { 0.0f,0.0f,0.0f };
	XMFLOAT3 headpart_rot_ = { 0.0f,0.0f,0.0f };
	XMFLOAT3 bodypart_rot_ = { 0.0f,0.0f,0.0f };
	//�U��
	float vibration_ = 0.0f;
	bool vibrationchange_flag_ = false;
	float atttack_timer_ = 0.f;
	float timer_limit_ = 0.f;
	//�G�̃��[�V�����p
	float purse_positiverot_ = 180;
	//�G�ƃv���C���[�̋���
	float originhead_distance_;
	float head_distance_ = 30.0f;
	//�ό`
	float defomation_count_ = 0.0f;

	float limit_length_ = 1.2f;
	//�����Ǐ]��ɕʂ̓G�����邩���Ȃ���
	bool wait_flag_ = false;
	float attack_charge_ = 0.0f;
};

