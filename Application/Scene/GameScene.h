#pragma once
#include<fstream>
#include<sstream>
#include<memory>
#include"BaseScene.h"
#include"Continue.h"
#include"EnemyPop.h"
#include"GameBackground.h"
#include"ModelManager.h"
#include"MovieStaging.h"
#include"Movie.h"
#include"Player.h"
#include"TitleScene.h"
#include"RailCamera.h"
#include"CommonBackground.h"
#include"LightControl.h"


class GameScene :public BaseScene
{
private:
	enum GamePhase {
		NONE = 0,
		START,
		FIGHT,
		MOVE,
		CONTINUE,
		CLEAR,
	};

	enum ENEMYPATERN {
		NORMAL=0,
		THROW,
		BOSS,
	};

private:
	//移動用の関数ポインタ	
	static void (GameScene::* MoveFuncTable[])();
public://メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="sceneManager_"></param>
	GameScene(SceneManager* sceneManager_);
	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="dxComon"></param>
	void Initialize(DirectXCommon* dxComon) override;
	/// <summary>
	/// ステータスセット
	/// </summary>
	void StatusSet();
	/// <summary>
	/// オブジェクト等の全ての更新処理
	/// </summary>
	void AllUpdata();
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;
	/// <summary>
	/// 始まりのカメラ演出
	/// </summary>
	void StartProcess();
	/// <summary>
	/// 移動時の処理
	/// </summary>
	void MoveProcess();

	void WarPointArrival(float manyvalue, float fewvalue);

	bool DestinationArrivalCheck(float manyvalue, float fewvalue);
	/// <summary>
	/// 戦闘時の処理
	/// </summary>
	void FightProcess();
	/// <summary>
	/// ゲームオーバー時の処理
	/// </summary>
	void GameOverProcess();
	/// <summary>
	/// ゲームクリア時の処理
	/// </summary>
	void GameClearProcesss();
	/// <summary>
	/// ゲームが動いている時の処理
	/// </summary>
	void MovingGameProcesss();
	/// <summary>
	/// 徐々に明るくする
	/// </summary>
	void FadeIn();
	/// <summary>
	/// スポットライトの動き
	/// </summary>
	void SpotLightMove();
	/// <summary>
	/// 攻撃を食らったときの処理
	/// </summary>
	void DamageProcess();
	/// <summary>
	/// オブジェクトの描画
	/// </summary>
	/// <param name="dxCommon">dxCommonからコマンドリストをゲット</param>
	void ObjDraw(DirectXCommon* dxCommon);
	/// <summary>
	/// スプライトの描画
	/// </summary>
	/// <param name="dxCommon">dxCommonからコマンドリストをゲット</param>
	void SpriteDraw(DirectXCommon* dxCommon);
	/// <summary>
	/// ImgUI描画
	/// </summary>
	void ImgDraw();
	/// <summary>
	/// ポストエフェクトの描画
	/// </summary>
	/// <param name="dxCommon">dxCommonからコマンドリストをゲット</param>
	void PostEffectDraw(DirectXCommon* dxCommon);
	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="dxCommon">dxCommonからコマンドリストをゲット</param>
	void Draw(DirectXCommon* dxCommon)override;
	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize() override;
public:
#pragma region 関数テーブル

	/// <summary>
	/// 第1地点後ろ
	/// </summary>
	void MoveStartBack();
	/// <summary>
	/// 第1地点前
	/// </summary>
	void MoveStartFront();

	void MovePointA();

	void MovePointALeft();

	void MovePointB();

	void MovePointC();

	void MovePointCOblique();

	void MovePointCFront();

	void GoalPointBack();

	void GoalPoint();


#pragma endregion

private://メンバ変数
	//Obj
	unique_ptr<Object3d> heri_;
	unique_ptr<Object3d> goal_;
	unique_ptr<Object3d> hane_;
	unique_ptr<CommonBackground> common_background_;
	unique_ptr<GameBackground>game_background_;
	//スプライト
	unique_ptr<Sprite> clear_;
	unique_ptr<Sprite> damageefectsprite_;
	unique_ptr<Sprite> shot_ = nullptr;
	unique_ptr<Sprite> reticleforgameover_;
	unique_ptr<Continue> continue_screen_;
	unique_ptr<Movie> movie_;
	//プレイヤーと敵
	unique_ptr<EnemyPop> enemypop_;
	unique_ptr<Player> player_;
	//その他の機能
	unique_ptr <Audio> bgm_ = nullptr;
	unique_ptr <Audio> herifry_ = nullptr;
	unique_ptr <PostEffect> posteffect_ = nullptr;
	unique_ptr<Light> light_ = nullptr;
	unique_ptr<LightGroup> lightgroupe_ = nullptr;
	unique_ptr<LightControl> lightcontrol_ = nullptr;
	unique_ptr<Camera> camera_ = nullptr;
	unique_ptr<RailCamera> railcamera_ = nullptr;
	unique_ptr<MovieStaging> moviestaging_ = nullptr;
	//ビル群のステータス
	XMFLOAT3 builsscl_ = { 10.0f,10.0f,10.0f };
	XMVECTOR builshighalphapos_ = { 0.0f,0.0f,-16.5f };
	XMVECTOR builslowalphapos_ = { 0.0f,0.0f,-16.5f };
	XMFLOAT3 builsrot_ = { 0.0f,90.0f,0.0f };
	//ヘリのステータス(スタートのムービーと終わりのムービー)
	//スタート
	XMVECTOR heripos_ = { 0.0f,75.0f,-160.0f };
	XMFLOAT3 heriscl_ = { 10.0f,10.0f,10.0f };
	//終わり
	XMVECTOR goalpos_ = { 56.f,3.0f,92.0f };
	XMFLOAT3 goalscl_ = { 3.0f,3.0f,3.0f };
	//その他の変数
	XMFLOAT4 postefectcolor_ = { -1.0f,-1.0f,-1.0f,0.0f };
	int patern_ = 0;
	int playerhp_;
	int oldhp_;
	bool posteffectonflag_ = false;
	bool damagehitflag_ = false;
	XMFLOAT4 damageefectcolor_ = { 1.f,1.f,1.f,1.f };

	XMFLOAT2 reticleposition_{ 0.f,0.f };
	XMFLOAT4 reticlecolor_{ 1.f,1.f,1.f,1.f };
	XMFLOAT2 spriteanchorpoint_{ 0.5f,0.5f };
	XMFLOAT2 yesposition_{ WinApp::window_width / 2 - 200,WinApp::window_height / 2 + 100 };
	XMFLOAT2 noposition_{ WinApp::window_width / 2 + 200,WinApp::window_height / 2 + 100 };
	XMFLOAT2 spritesize_{ 240.f,240.f };
	XMFLOAT4 yescolor_{ 1.f,1.f,1.f,1.f };
	XMFLOAT4 nocolor_{ 1.f,1.f,1.f,1.f };
	bool yescursorinflag_ = false;
	bool nocursorinflag_ = false;
	float heriy_ = 15.0f;
	bool stopflag_ = false;
	XMVECTOR velocity_;
	XMFLOAT3 eyerot_{ 0.f,180.f,0.f };
	XMFLOAT3 passrot_{};
	XMVECTOR cameravector_{ 0.f,0.f,0.f };
	float movespeed_ = 0.5f;
	float changerotation_ = 0.f;
	bool FringFlag = false;

	bool shakingstartflag_ = false;
	float shake_addvalue_ = 0.f;
	//ステータス
	int gamestate_ = GamePhase::NONE;
	int movie_sequence_ = MovieSequence::kAction;
};

