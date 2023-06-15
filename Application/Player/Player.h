#pragma once
#include"Object3d.h"
#include"Sprite.h"
#include<vector>
#include"Audio.h"
#include"BulletUI.h"
#include"HelperMath.h"
#include"ParticleManager.h"
#include"PlayerUI.h"
#include"RailCamera.h"
#include<memory>
#include<list>

enum  Phase {
	kLandingPointBack = 0,
	kLandingPointFront,
	kMovedPointA,
	kMovedPointALeft,
	kMovedPointB,
	kMovedPointC,
	kMovedPointCOblique,
	kMovedPointCFront,
	kGoalPointBack,
	kGoalPoint,

};

class Player
{
private:
	enum State {
		kWait = 0,
		kReload,
		kShot,
	} player_state_;
private:
	static void (Player::* StateFunctable[])();
public:
	~Player();
	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="came">カメラ</param>
	void Initalize(Camera* camera);
	/// <summary>
	/// ステータスのSetは全部ここ
	/// </summary>
	/// <param name="came"></param>
	void StatusSet(Camera* camera, XMFLOAT3 eyerot);
	/// <summary>
	///　オブジェクト等の更新処理
	/// </summary>
	void AllUpdate();
	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="camera">カメラ</param>
	/// <param name="patern">ゲームのフェイズ</param>
	/// <param name="eyerot">カメラの角度</param>
	/// <param name="gamestate">ゲームの状態</param>
	/// <param name="state">ゲーム開始の値</param>
	void Update(Camera* camera, Phase patern,XMFLOAT3 eyerot,int gamestate,int state);
	/// <summary>
	/// 待機時の処理
	/// </summary>
	void WaitProcess();
	/// <summary>
	/// パーティクルの描画
	/// </summary>
	/// <param name="cmdeList">コマンドリスト</param>
	void ParticleDraw(ID3D12GraphicsCommandList* cmdeList);
	/// <summary>
	/// スプライトの描画
	/// </summary>
	void SpriteDraw();
	/// <summary>
	/// Objの描画
	/// </summary>
	void ObjDraw();
	/// <summary>
	/// SE呼び出し
	/// </summary>
	void SoundEffect();
	/// <summary>
	/// マウスの取得
	/// </summary>
	void MouseContoroll();
	/// <summary>
	/// 発砲処理
	/// </summary>
	void GunShotProcess();
	/// <summary>
	/// UIの動き処理
	/// </summary>
	void UIMotionProcess();
	/// <summary>
	/// リコイルの処理
	/// </summary>
	void RecoilProcess();
	/// <summary>
	/// リロード処理
	/// </summary>
	void ReloadProcess();
	/// <summary>
	/// パーティクル発生
	/// </summary>
	void ParticleEfect(int paterncount);
	/// <summary>
	/// 徐々に大きくする
	/// </summary>
	void SlowlyLargeHUD();
	/// <summary>
	/// 徐々に小さくする
	/// </summary>
	void SlowlySmallHUD();
	/// <summary>
	/// Imguiの描画
	/// </summary>
	void ImGuiDraw();
public:
#pragma region Get
	//三次元座標
	const XMVECTOR& GetPosition() { return reticle_pos_; }
	//角度
	const XMFLOAT3& GetRotation() { return reticle_rot_; }
	//スケール
	const XMFLOAT3& GetScl() { return reticle_scl_; }
	//二次元座標
	const XMFLOAT2& GetRetPosition() { return reticle_pos2d_; }
	//二次元スケール
	const XMFLOAT2& GetRetSiz() { return reticle_size_; }
	//Hp
	int GetHp() { return hp_; }
	//撃った時
	const bool& GetBulletShot() { return bullet_shotflag_; }

	const XMVECTOR& GetBodyWorldPos() { return body_worldpos_; }

	const XMMATRIX& GetBodyMatrix() { return body_mat_; }
#pragma endregion

#pragma region Set
	void SetPosition(const XMVECTOR& position) { this->reticle_pos_ = position; }
	void SetRotation(const XMFLOAT3& rotation) { this->reticle_rot_ = rotation; }
	void SetScl(const XMFLOAT3& scale) { this->reticle_scl_ = scale; }
	void SetRetPosition(const XMFLOAT2& position) { this->reticle_pos2d_ = position; }
	void SetRetSiz(const XMFLOAT2& scale) { this->reticle_size_ = scale; }
	void SetHp(int HP) { this->hp_ = HP; }
	void SetBulletShot(const bool& bulletshotflag) { this->bullet_shotflag_ = bulletshotflag; }
	void SetBodyWorldPos(const XMVECTOR& worldbodypos) { this->body_worldpos_ = worldbodypos; }
#pragma endregion
private:
	//Obj
	unique_ptr<Object3d> gun_;
	unique_ptr<Object3d> body_;
	//スプライト
	unique_ptr<Sprite> sprite_reticle_;
	unique_ptr<BulletUI>bullet_ui_;
	unique_ptr<PlayerUI> player_ui_;
	//発砲時のエフェクト
	unique_ptr <ParticleManager> part_green_ = nullptr;
	unique_ptr <ParticleManager> part_red_ = nullptr;
	unique_ptr <ParticleManager> part_smoke_ = nullptr;
	//その他の機能
	unique_ptr<Audio> shot_se_;
	unique_ptr<Audio> reload_se_;
	//ローカル
		//レティクルObjのステータス
	XMVECTOR reticle_pos_ = { 0.0f,55.0f,0.0f };
	XMFLOAT3 reticle_rot_ = { 0.0f,0.0f,0.0f };
	XMFLOAT3 reticle_scl_ = { 0.3f,0.3f,0.3f };
	//発射台Objのステータス
	XMVECTOR gun_pos_ = { 0.0f,0.0f,-3.0f };
	XMVECTOR gun_notparentpos_ = { 0.0f,0.0f,-10.0f };
	XMFLOAT3 gun_rot_ = { 0.0f,0.0f,0.0f };
	XMFLOAT3 gun_scl_ = { 0.3f,0.3f,0.3f };
	//プレイヤーObjのステータス
	XMVECTOR body_pos_ = { 0.0f,0.0f,0.0f };
	XMFLOAT3 body_rot_ = { 0.0f,0.0f,0.0f };
	XMFLOAT3 body_scl_ = { 0.3f,0.3f,0.3f };
	//ワールド
		//レティクルのステータス
	XMVECTOR track_worldpos_ = { 0.0f,0.0f,0.0f };
	XMMATRIX track_mat_;
	XMVECTOR world_farpos_ = { 0.0f,0.0f,0.0f };
	//発射台のステータス
	XMVECTOR gun_worldpos_ = { 0.0f,0.0f,-5.0f };
	XMMATRIX gun_mat_;
	//プレイヤーのステータス
	XMVECTOR body_worldpos_ = { 0.0f,0.0f,-0.1f };
	XMMATRIX body_mat_;
	XMVECTOR reticle_worldpos_;
	//パーティクルのステータス
	XMVECTOR particle_pos_ = { 0.0f,0.0f,2.0f };
	//カメラ
	XMFLOAT3 eye_rot_ = { 0.0f,180.0f,0.0f };
	XMFLOAT3 target_pos_ = { 0.0f,0.0f,0.0f };
	XMFLOAT3 up_ = { 0.0f,1.0f,0.0f };
	//スプライト
	//共通のアンカーポイント
	XMFLOAT2 anchorpoint_ = { 0.5f,0.5f };
		//2Dレティクルのステータス
	XMFLOAT4 reticle_color_ = { 1,1,1,1 };
	XMFLOAT2 ancorpoint_ = { 0.5f,0.5f };
	XMFLOAT2 reticle_pos2d_ = { 640.0f,360.0f };
	XMFLOAT2 reticle_size_ = { 64.0f,64.0f };
	//始まりと終わりの演出使う黒いカーテン
	XMFLOAT2 curtain_uppos_ = { 0.0f,0.0f };
	XMFLOAT2 curtain_size_ = { 1280.0f,100.0f };
	XMFLOAT2 curtain_downpos_ = { 0.0f,620.0f };
	//Reloadのステータス
	XMFLOAT4 reload_spritecolor_ = { 1.0f,1.0f,1.0f,0.9f };
	XMFLOAT2 reload_spritepos_ = { WinApp::window_width / 2,210 };
	XMFLOAT2 reload_spritesize_ = { 210,140 };
	//UIのステータス
	XMFLOAT2 ui_bulletpos{ 1220.0f,25.0f  };
	XMFLOAT2 ui_reloadpos_{ WinApp::window_width / 2,210 };
	//Skip文字の表示
	XMFLOAT2 skippos_ = curtain_downpos_;
	//2D座標入の取得用変数
	XMVECTOR offset_ = { 0,0,1.0f };
	//プレイヤーのHp
	int hp_ = 5;
	int old_hp_ = hp_;
	//Reload
	int reload_time_ = 0;
	//残弾数
	int remaining_ = 0;
	int old_remaining_ = 0;
	//カメラのeyeを代入し、マズルフラッシュの表示に使用
	XMFLOAT3 change_rot_{};
	//弾の発射の処理
	bool bullet_shotflag_ = false;
	float shot_cooltime_ = 0.0f;
	//リコイルの処理に使われるもの
	float recoil_time_ = 0.0f;
	float recovery_time_ = 0.0f;
	bool recoil_gunflag_ = false;
	//UIの大きさが一定まで行ったときにフラグを立てる
	bool revers_flag_ = false;
	XMFLOAT3 pos{};
	int game_phase_{};
};

