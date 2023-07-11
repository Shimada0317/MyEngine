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
    /// コンストラクタ
    /// </summary>
    /// <param name="allrot"></param>
    /// <param name="allpos"></param>
    /// <param name="trackpoint"></param>
    Low(const XMFLOAT3& allrot, const XMVECTOR& allpos, const XMVECTOR& trackpoint);
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
    /// <summary>
    /// 倒されているか
    /// </summary>
    /// <returns></returns>
    bool IsDead() const { return dead_flag_; }
private:
    /// <summary>
    /// 変形
    /// </summary>
    void Defomation();
    /// <summary>
    /// プレイヤー追尾モード
    /// </summary>
    void TrackPlayerMode();
    /// <summary>
    /// 死んだ
    /// </summary>
    void Death();
    /// <summary>
    /// 展開時の大きさ
    /// </summary>
    void DeploymentScale();
    /// <summary>
    /// 透明にする
    /// </summary>
    void Transparentize();
    /// <summary>
    /// 攻撃を受けたときに色を変える
    /// </summary>
    void HitColor();

    void EnemyCollsion();
private:
    //Obj
    //頭
    std::unique_ptr<Object3d> headpart_;
    //両腕
    std::unique_ptr<Object3d> armspart_;
    //両足
    std::unique_ptr<Object3d> legspart_;
    //パーツごとの回転
    XMFLOAT3 headpart_rot_ = { 0.0f,0.0f,0.0f };
    XMFLOAT3 armspart_rot_ = { 0.0f,0.0f,0.0f };
    //パーツごとのスケール
    XMFLOAT3 headpart_scl_ = { 0.2f,0.2f,0.2f };
    XMFLOAT3 armspart_scl_ = { 0.1f,0.1f,0.1f };
    //パーツごとのポジション
    XMVECTOR headpart_pos_ = { 0.0f,0.0f,0.0f };
    XMVECTOR armspart_pos_ = { 0.0f,0.0f,0.0f };
    XMVECTOR legspart_pos_ = { 0.0f,0.0f,0.0f };
    //パーツごとの色
    XMFLOAT4 headpart_color_ = { 1.0f,1.0f,1.0f,1.0f };
    XMFLOAT4 armspart_color_ = { 1.0f,1.0f,1.0f,1.0f };
    //変形用のタイマー
    float defomation_count_ = 0.0f;
    //敵とプレイヤーの距離
    float originhead_distance_;
    float head_distance_ = 30.0f;
    float state_timer_ = 0.f;
    float gravity_timer_ = 1.f;
};

