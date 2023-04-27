#pragma once
#include<memory>
#include"BaseScene.h"
#include"GameScene.h"
#include"BulletUI.h"

const int BUILSAMOUNT = 16;

class TitleScene :public BaseScene
{
private:
	enum TITLESTATE {
		TITLESCREEN=0,
		DESCRIPTIONPAGE,
		ENEMYOVERVIEWPAGE,
		GAMESTARTPREPARTIONPAGE,
	};

	enum UISTATE {
		WAIT=0,
		RELOAD,
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

	void ReloadProcess();

	void ShotProcess();

	void DescriptioPageProcess();

	void GameStartPrepartionPage();

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize() override;
public://関数テーブル



private:
	//Obj
	unique_ptr <Object3d> sphere_;
	unique_ptr<Object3d> builshighalpha_[BUILSAMOUNT];
	unique_ptr<Object3d> builslowalpha_[BUILSAMOUNT];
	unique_ptr <Object3d >world_;
	unique_ptr<Object3d> start_;
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
	unique_ptr<Light> light_ = nullptr;
	unique_ptr<LightGroup> lightgroupe_ = nullptr;
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
	XMFLOAT3 spotlightdir_ = { 0,-1,0 };
	XMFLOAT3 spotlightpos_ = { 0,1005,0 };
	XMFLOAT3 spotlightcolor_ = { 1,1,1 };
	XMFLOAT3 spotlightatten_ = { 0.f,0.f,0.f };
	XMFLOAT2 spotlightfactorangle_ = { 20.0f,30.0f };
	XMFLOAT3 spotlightdir2_ = { 0,0,0 };
	XMFLOAT3 spotlightpos2_ = { 10,1005,160 };
	XMFLOAT3 spotlightcolor2_ = { 0,0,1 };
	XMFLOAT3 spotlightatten2_ = { 0.f,0.f,0.f };
	XMFLOAT2 spotlightfactorangle2_ = { 20.0f,30.0f };

	float easingtimer_ = 0.f;
	bool easingchangeflag_=false;
	XMFLOAT2 clicksize_ = { 550.f,60.f };

	int remaining_ = 0;
	int old_remaining_ = 0;
	bool revers_flag_ = false;
	int reloadtime_ = 0;

	int hudstate_ = WAIT;
	int titlestate_ = TITLESCREEN;
};

