#pragma once
#include"Object3d.h"
#include"Sprite.h"
#include<vector>
#include"Audio.h"
#include"HelperMath.h"
#include"ParticleManager.h"
#include"RailCamera.h"
#include<memory>
#include<list>

enum  Phase {
	LANDINGPOINT_BACK = 0,
	LANDINGPOINT_FRONT,
	MOVEDPOINT_A,
	MOVEDPOINT_A_LEFT,
	MOVEDPOINT_B,
	MOVEDPOINT_C,
	MOVEDPOINT_C_OBLIQUE,
	MOVEDPOINT_C_FRONT,
	GOALPOINT_BACK,
	GOALPOINT,

};

class Player
{
private:
	enum State {
		WAIT = 0,
		RELOAD,
		SHOT,
		MOVE,
	};
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
	/// <param name="bull">プレイヤーの弾</param>
	/// <param name="Remaining">残弾数</param>
	/// <param name="enePos">敵の座標</param>
	/// <param name="came">カメラ</param>
	/// <param name="Ene2dPos">敵の2D座標</param>
	/// <param name="pat">現在のフェイズ</param>
	void Update(Camera* camera, Phase patern,XMFLOAT3 eyerot);
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
	void GunShotProcess(Phase paterncount);
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
	void ParticleEfect(Phase paterncount);
	/// <summary>
	/// Imguiの描画
	/// </summary>
	void ImGuiDraw();
public:
#pragma region Get
	//三次元座標
	const XMVECTOR& GetPosition() { return reticlepos_; }
	//角度
	const XMFLOAT3& GetRotation() { return reticlerot_; }
	//スケール
	const XMFLOAT3& GetScl() { return reticlescl_; }
	//二次元座標
	const XMFLOAT2& GetRetPosition() { return reticlepos2d_; }
	//二次元スケール
	const XMFLOAT2& GetRetSiz() { return reticlesize_; }
	//Hp
	int GetHp() { return hp_; }
	//撃った時
	const bool& GetBulletShot() { return bulletshotflag_; }

	const XMVECTOR& GetBodyWorldPos() { return bodyworldpos_; }

	const XMVECTOR& GetVelocity() { return velocity_; }

	const XMMATRIX& GetBodyMatrix() { return bodymat_; }
#pragma endregion

#pragma region Set
	void SetPosition(const XMVECTOR& position) { this->reticlepos_ = position; }
	void SetRotation(const XMFLOAT3& rotation) { this->reticlerot_ = rotation; }
	void SetScl(const XMFLOAT3& scale) { this->reticlescl_ = scale; }
	void SetRetPosition(const XMFLOAT2& position) { this->reticlepos2d_ = position; }
	void SetRetSiz(const XMFLOAT2& scale) { this->reticlesize_ = scale; }
	void SetHp(int HP) { this->hp_ = HP; }
	void SetBulletShot(const bool& BulletShot_F) { this->bulletshotflag_ = BulletShot_F; }
	void SetBodyWorldPos(const XMVECTOR& worldbodypos) { this->bodyworldpos_ = worldbodypos; }
#pragma endregion
private:
	//Obj
	unique_ptr<Object3d> gun_;
	unique_ptr<Object3d> body_;
	//スプライト
	unique_ptr<Sprite> spritereticle_;
	unique_ptr<Sprite> bulletHUD[9];
	unique_ptr<Sprite> reload_;
	//発砲時のエフェクト
	unique_ptr <ParticleManager> partgreen_ = nullptr;
	unique_ptr <ParticleManager> partred_ = nullptr;
	unique_ptr <ParticleManager> partsmoke_ = nullptr;
	//その他の機能
	unique_ptr<Audio> shotse_;
	unique_ptr<Audio> reloadse_;
	//ローカル
		//レティクルObjのステータス
	XMVECTOR reticlepos_ = { 0.0f,55.0f,0.0f };
	XMFLOAT3 reticlerot_ = { 0.0f,0.0f,0.0f };
	XMFLOAT3 reticlescl_ = { 0.3f,0.3f,0.3f };
	//発射台Objのステータス
	XMVECTOR gunpos_ = { 0.0f,0.0f,-3.0f };
	XMVECTOR gunnotparentpos_ = { 0.0f,0.0f,-10.0f };
	XMFLOAT3 gunrot_ = { 0.0f,0.0f,0.0f };
	XMFLOAT3 gunscl_ = { 0.3f,0.3f,0.3f };
	//プレイヤーObjのステータス
	XMVECTOR bodypos_ = { 0.0f,0.0f,0.0f };
	XMFLOAT3 bodyrot_ = { 0.0f,0.0f,0.0f };
	XMFLOAT3 bodyscl_ = { 0.3f,0.3f,0.3f };
	//ワールド
		//レティクルのステータス
	XMVECTOR trackworldpos_ = { 0.0f,0.0f,0.0f };
	XMMATRIX trackmat_;
	XMVECTOR worldfarpos_ = { 0.0f,0.0f,0.0f };
	//発射台のステータス
	XMVECTOR gunworldpos_ = { 0.0f,0.0f,-5.0f };
	XMMATRIX gunmat_;
	//プレイヤーのステータス
	XMVECTOR bodyworldpos_ = { 0.0f,0.0f,-0.1f };
	XMMATRIX bodymat_;
	XMVECTOR reticleworldpos_;
	//パーティクルのステータス
	XMVECTOR particlepos_ = { 0.0f,0.0f,2.0f };
	//カメラ
	XMFLOAT3 eyerot_ = { 0.0f,180.0f,0.0f };
	XMFLOAT3 targetpos_ = { 0.0f,0.0f,0.0f };
	XMFLOAT3 up_ = { 0.0f,1.0f,0.0f };
	//スプライト
		//2Dレティクルのステータス
	XMFLOAT4 reticlecolor_ = { 1,1,1,1 };
	XMFLOAT2 reticleancorpoint_ = { 0.5f,0.5f };
	XMFLOAT2 reticlepos2d_ = { 640.0f,360.0f };
	XMFLOAT2 reticlesize_ = { 64.0f,64.0f };
	//始まりと終わりの演出使う黒いカーテン
	XMFLOAT2 curtainuppos_ = { 0.0f,0.0f };
	XMFLOAT2 curtainsize_ = { 1280.0f,100.0f };
	XMFLOAT2 curtaindownpos_ = { 0.0f,620.0f };
	//Skip文字の表示
	XMFLOAT2 skippos_ = curtaindownpos_;
	//2D座標入の取得用変数
	XMVECTOR offset_ = { 0,0,1.0f };

	int hp_ = 5;
	int oldhp_ = hp_;
	//Reload
	int reloadtime_ = 0;
	//移動
	XMVECTOR velocity_;
	//cam
	int actioncount_ = 0;
	float actiontimer_ = 0.0f;

	bool shakeheadflag_ = true;

	XMFLOAT3 changerot_{};


	bool bulletshotflag_ = false;
	float shotcooltime_ = 0.0f;
	bool recoilflag_ = false;
	float recoiltime_ = 0.0f;

	bool mousestopflag_ = false;

	bool recoilgunflag_ = false;
	float recoverytime_ = 0.0f;
	//持ってきたもの
	int remaining_ = 0;
	int oldremaining_ = 0;

	XMFLOAT2 spritepos_[9];
	bool dropbulletflag_[9];
	float spriterot_[9];

	float time_[9];


	//Spriteのステータス
	XMFLOAT4 reloadspritecolor_ = { 1.0f,1.0f,1.0f,0.9f };
	XMFLOAT2 anchorpoint_ = { 0.5f,0.5f };
	XMFLOAT2 reloadspritepos_ = { WinApp::window_width / 2,210 };
	XMFLOAT2 reloadspritesize_ = { 210,140 };

	bool reversflag_ = false;

	int playerstate_ = WAIT;
};

