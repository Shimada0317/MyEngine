#pragma once
#include "BaseEnemy.h"
class Low :
    public BaseEnemy
{
private:
    enum State {
        kDefomation=0,
        kMove,
        kDeath,
    }state_;


    enum Move {
        kNormal=0,
        kJump,
    }move_state_;
private:
    static void (Low::* StateFuncTable[])();
public:
    /// <summary>
    /// �R���X�g���N�^
    /// </summary>
    /// <param name="allrot"></param>
    /// <param name="allpos"></param>
    /// <param name="trackpoint"></param>
    Low(const XMFLOAT3& allrot, const XMVECTOR& allpos, const XMVECTOR& trackpoint);
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
    /// <summary>
    /// �|����Ă��邩
    /// </summary>
    /// <returns></returns>
    bool IsDead() const { return dead_flag_; }
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
    /// ����
    /// </summary>
    void Death();
    /// <summary>
    /// �W�J���̑傫��
    /// </summary>
    void DeploymentScale();
    /// <summary>
    /// �����ɂ���
    /// </summary>
    void Transparentize();
    /// <summary>
    /// �U�����󂯂��Ƃ��ɐF��ς���
    /// </summary>
    void HitColor();

    void EnemyCollsion();
private:
    //Obj
    //��
    std::unique_ptr<Object3d> headpart_;
    //���r
    std::unique_ptr<Object3d> armspart_;
    //����
    std::unique_ptr<Object3d> legspart_;
    //�p�[�c���Ƃ̉�]
    XMFLOAT3 headpart_rot_ = { 0.0f,0.0f,0.0f };
    XMFLOAT3 armspart_rot_ = { 0.0f,0.0f,0.0f };
    //�p�[�c���Ƃ̃X�P�[��
    XMFLOAT3 headpart_scl_ = { 0.2f,0.2f,0.2f };
    XMFLOAT3 armspart_scl_ = { 0.1f,0.1f,0.1f };
    //�p�[�c���Ƃ̃|�W�V����
    XMVECTOR headpart_pos_ = { 0.0f,0.0f,0.0f };
    XMVECTOR armspart_pos_ = { 0.0f,0.0f,0.0f };
    XMVECTOR legspart_pos_ = { 0.0f,0.0f,0.0f };
    //�p�[�c���Ƃ̐F
    XMFLOAT4 headpart_color_ = { 1.0f,1.0f,1.0f,1.0f };
    XMFLOAT4 armspart_color_ = { 1.0f,1.0f,1.0f,1.0f };
    //�ό`�p�̃^�C�}�[
    float defomation_count_ = 0.0f;
    //�G�ƃv���C���[�̋���
    float originhead_distance_;
    float head_distance_ = 30.0f;
    float state_timer_ = 0.f;
    float gravity_timer_ = 1.f;
};

