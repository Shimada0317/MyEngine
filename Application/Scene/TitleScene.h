#pragma once
#include<memory>
#include"BaseScene.h"
#include"GameScene.h"
#include"BulletUI.h"
#include"LightControl.h"
#include"CommonBackground.h"


class TitleScene :public BaseScene
{
private:
	enum TITLESTATE {
		kTitleScreen=0,
		kDescriptionPage,
		kEnemyOverviewPage,
		kGameStartPreoartionPage,
	};

	enum UISTATE {
		kWait=0,
		kReload,
	};

private:
	static const int debugTextNumber = 0;
private:
	static void(TitleScene::* PageFuncTable[])();
public://メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="sceneManager_"></param>
	TitleScene(SceneManager* sceneManager_);
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
	/// Obj等のUpdateはこの中で
	/// </summary>
	void AllUpdate();
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;
	/// <summary>
	/// カメラの移動
	/// </summary>
	void CameraDirection();
	/// <summary>
	/// UIをイージングで拡大縮小させる処理
	/// </summary>
	void UiEasingProcess();
	/// <summary>
	/// 説明画面で行われる処理
	/// </summary>
	void ArrowProces();
	/// <summary>
	///　フェードアウト後にゲームシーンへチェンジ
	/// </summary>
	void FadeOutAndSceneChange();
	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="dxCommon"></param>
	void Draw(DirectXCommon* dxCommon)override;
	/// <summary>
	/// リロード処理
	/// </summary>
	void ReloadProcess();
	/// <summary>
	/// 発砲の処理
	/// </summary>
	void ShotProcess();
	/// <summary>
	/// 説明のページの処理
	/// </summary>
	void DescriptioPageProcess();
	/// <summary>
	/// ゲーム開始のページの処理
	/// </summary>
	void GameStartPrepartionPage();
	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize() override;
private:
	//Obj
	unique_ptr<CommonBackground> common_background_;
	//スプライト
	unique_ptr<Sprite> title_ = nullptr;
	unique_ptr<Sprite> cursor_ = nullptr;
	unique_ptr<Sprite> clickbefore_ = nullptr;
	unique_ptr<Sprite> clickafter_ = nullptr;
	unique_ptr<Sprite> signalbefore_ = nullptr;
	unique_ptr<Sprite> signalafter_ = nullptr;
	unique_ptr<Sprite> descriptionoperation_ = nullptr;
	unique_ptr<Sprite> enemyoverview_ = nullptr;
	unique_ptr<Sprite> gamestartpreparation_ = nullptr;
	unique_ptr<Sprite> arrowright_ = nullptr;
	unique_ptr<Sprite> arrowleft_ = nullptr;
	unique_ptr<BulletUI> bullet_ui_;
	//その他の機能
	unique_ptr<LightControl> lightcontrol_ = nullptr;
	unique_ptr<PostEffect> posteffct_ = nullptr;
	unique_ptr<Audio> clickse_ = nullptr;
	unique_ptr<Audio> bgm_ = nullptr;
	unique_ptr<Camera> titlecamera_ = nullptr;
	//球体のステータス
	XMVECTOR spherepos_ = { 0,0,0 };
	XMFLOAT3 spherescl_ = { 4.0f,4.0f,4.0f };
	XMFLOAT3 sphererot_ = { 0,0,0 };
	//ビル群のステータス
	XMFLOAT3 builsscl_ = { 10.0f,10.0f,10.0f };
	XMVECTOR builshighalphapos_ = { 0.0f,0.0f,-16.5f };
	XMVECTOR builslowalphapos_ = { 0.0f,0.0f,-16.5f };
	XMFLOAT3 builsrot_ = { 0.0f,90.0f,0.0f };
	//カメラの移動先のステータス
	XMVECTOR startpos_ = { 0.0f,0.0f,-16.5f };
	XMFLOAT3 startscl_ = { 15.0f,15.0f,15.0f };
	//カメラのステータス
	XMVECTOR cameramove_ = { 0.0f,0.0f,0.0f };
	XMVECTOR cameraeyemove_ = { 0.0f,0.0f,0.0f };
	bool cameraeyemoveflag_ = false;
	bool camerachangeflag_ = false;
	//足場のステータス
	XMVECTOR worldpos_ = { 0.0f,-1.1f,0.0f };
	XMFLOAT3 worldscl_ = { 100,100,100 };
	//スプライトのステータス
	XMFLOAT4 spritecol_ = { 1,1,1,1 };
	XMFLOAT2 anchorpoint_ = { 0.5f,0.5f };
	XMFLOAT2 reticlepos_ = { 640.0f,360.0f };
	XMFLOAT2 clickpos_ = { WinApp::window_width/2,600 };
	XMFLOAT2 clicksize_ = { 550.f,60.f };
	//タイトルスプライトのステータス
	XMFLOAT2 titlesize_ = { 1280.0f,720.0f };
	XMFLOAT2 titlepos_ = { 0.0f,0.0f };
	bool titlespriteflag_ = true;
	//UIのステータス
	XMFLOAT2 ui_bulletpos_{ 1120.0f,45.0f };
	XMFLOAT2 ui_reloadpos_ = { 900,410 };
	//説明の矢印座標
	XMFLOAT2 arrowrightpos_ = { 1240.0f,300 };
	XMFLOAT2 arrowleftpos_ = { 35.0f,300 };
	XMFLOAT2 arrowsize_ = { 32.f,32.f};
	XMFLOAT4 arrowrightcolor_ = { 1.f,1.f,1.f,1.f };
	XMFLOAT4 arrowleftcolor_ = { 1.f,1.f,1.f,1.f };
	bool righttrueinflag_ = false;
	bool lefttrueinflag_= false;
	//ポストエフェクトの色
	XMFLOAT4 postefectcolor_ = { 0.0f,0.0f,0.0f,1.0f };
	//カーソルが入っているか
	bool signalcurorinflag_ = false;
	//フェードを掛けるか掛けないか
	bool fadeoutflag_ = false;
	//クリックしているかどうか
	bool clickflag_ = true;
	//音量
	float volume_ = 0.8f;
	//ライトのステータス
	XMFLOAT3 spotlightpos_ = { 0,1005,0 };
	XMFLOAT3 spotlightcolor_ = { 1,1,1 };
	//イージング変数
	float easingtimer_ = 0.f;
	bool easingchangeflag_=false;
	//残用の変数
	int remaining_ = 0;
	int old_remaining_ = 0;
	bool revers_flag_ = false;
	int reloadtime_ = 0;
	//ステータス
	int hudstate_ = kWait;
	int titlestate_ = kTitleScreen;
};

