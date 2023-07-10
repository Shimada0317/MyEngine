#pragma once
#include "BaseEnemy.h"
class Throw :
    public BaseEnemy
{
private:
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
    //状態遷移の関数ポインタ
    static void (Throw::* StateFuncTable[])();
public:
    Throw(const XMFLOAT3& allrot, const XMVECTOR& allpos, const XMVECTOR& trackpoint);
    /// <summary>
    /// 生成
    /// </summary>
    /// <param name="camera"></param>
    void CreateRobot(Camera* camera)override;
    /// <summary>
    /// ステータスのセット
    /// </summary>
    void StatusSet();
    /// <summary>
    /// 全ての更新処理
    /// </summary>
    void AllUpdate();
    /// <summary>
    /// 行動
    /// </summary>
    /// <param name="player"></param>
    void Activity(Player* player)override;
    /// <summary>
    /// 描画
    /// </summary>
    /// <param name="dxCommon"></param>
    void Draw(DirectXCommon* dxCommon)override;
private:
    /// <summary>
    /// 登場処理
    /// </summary>
    void AppearanceProcess();
    /// <summary>
    /// 待機処理
    /// </summary>
    void WaitProcess();
    /// <summary>
    ///	攻撃処理
    /// </summary>
    void AttackProcess();
    /// <summary>
    /// 死亡処理
    /// </summary>
    void DeathProcess();
private:
    unique_ptr<Object3d> bullet_;
    unique_ptr<Object3d> body_;
    unique_ptr<Object3d> propeller_;
    //本体のステータス
    XMVECTOR body_pos_ = {};
    XMFLOAT3 body_rot_ = {};
    XMFLOAT3 body_scl_ = { 1.f,1.f,1.f };
    //羽のステータス
    XMVECTOR propeller_pos_{ 0.f,0.f,0.f };
    XMFLOAT3 propeller_rot_{ 0.f,0.f,0.f };
    XMFLOAT3 propeller_scl_{ 1.f,1.f,1.f };
    //弾のステータス
    XMVECTOR old_pos_{};
    XMVECTOR bullet_pos_{};
    XMFLOAT3 bullet_rot_{};
    XMFLOAT3 bullet_scl_{};

    XMFLOAT3 body_rot_ = {};
    XMFLOAT3 body_scl_ = { 1.f,1.f,1.f };
    //着弾地点
    XMVECTOR landing_point_ = {};
    //色
    XMFLOAT4 color_ = { 1.f,1.f,1.f,1.f };
    XMFLOAT4 bullet_color_ = { 1.f,0.f,0.f,1.f };
};

