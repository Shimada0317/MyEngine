#pragma once
#include "BaseEnemy.h"
class ThrowEnemy :
    public BaseEnemy
{
private:
    enum Type {
        kNormal = 1,
        kRed,
        kGreen,
        kBlue,
    }type_;
    enum State {
        kAppearance=0,
        kWait,
        kAttack,
        kDeath,
    }state_;

    enum BulletState {
        kShotBefore=0,
        kShotAfter,
    }bullet_state_;
private:
    //��ԑJ�ڂ̊֐��|�C���^
    static void (ThrowEnemy::* StateFuncTable[])();
public:
    ThrowEnemy(const XMFLOAT3& allrot, const XMVECTOR& allpos, const XMVECTOR& trackpoint,int type);
    /// <summary>
    /// ����
    /// </summary>
    /// <param name="camera"></param>
    void CreateRobot(Camera* camera)override;
    /// <summary>
    /// �X�e�[�^�X�̃Z�b�g
    /// </summary>
    void StatusSet();
    /// <summary>
    /// �S�Ă̍X�V����
    /// </summary>
    void AllUpdate();
    /// <summary>
    /// �s��
    /// </summary>
    /// <param name="player"></param>
    void Activity(Player* player)override;
    /// <summary>
    /// �`��
    /// </summary>
    /// <param name="dxCommon"></param>
    void Draw(DirectXCommon* dxCommon)override;
private:
    /// <summary>
    /// �e�̓����蔻��
    /// </summary>
    void BulletCollision();
    /// <summary>
    /// �U�����@
    /// </summary>
    void ThrowAttack();
    /// <summary>
    /// �����̌v�Z
    /// </summary>
    void RangeCalculation();
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
    /// <summary>
    /// �����蔻��
    /// </summary>
    void EnemyCollision();
private:
    unique_ptr<Object3d> bullet_;
    unique_ptr<Object3d> body_;
    unique_ptr<Object3d> propeller_;
    //�{�̂̃X�e�[�^�X
    XMVECTOR body_pos_ = {};
    XMFLOAT3 body_rot_ = {};
    XMFLOAT3 body_scl_ = { 1.f,1.f,1.f };
    //�H�̃X�e�[�^�X
    XMVECTOR propeller_pos_{ 0.f,0.f,0.f };
    XMFLOAT3 propeller_rot_{ 0.f,0.f,0.f };
    XMFLOAT3 propeller_scl_{ 1.f,1.f,1.f };
    //�e�̃X�e�[�^�X
    XMVECTOR old_pos_{};
    XMVECTOR bullet_pos_{};
    XMFLOAT3 bullet_rot_{};
    XMFLOAT3 bullet_scl_{};
    //���e�n�_
    XMVECTOR landing_point_ = {};
    //�F
    XMFLOAT4 color_ = { 1.f,1.f,1.f,1.f };
    XMFLOAT4 bullet_color_ = { 1.f,0.f,0.f,1.f };
    //�̂ɂ���ĕς��l
    float floating_pos_ = 0.f;
    float fall_time_ = 0.f;
    float bullet_distance_ = 0.f;
    float bullet_value_ = 0.f;
    float body_length_ = 0.f;
    float track_length_ = 0.f;
    float sub_scl_ = -0.001f;
    float timer_ = 0.f;
    float fall_speed_ = 0.15f;
    float add_scl_ = 0.005f;
};

