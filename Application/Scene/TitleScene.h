#pragma once
#include<memory>
#include"BaseScene.h"
#include"GameScene.h"

const int BUILSAMOUNT = 16;

class TitleScene :public BaseScene
{
private:
	enum OperationPage {
		DescriptionPage = 0,
		EnemyOverViewPage,
		GameStartPrepartionPage,
	};

private:
	static const int debugTextNumber = 0;

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
	/// カーソルが範囲内に入っているか
	/// </summary>
	/// <param name="cursorPos">カーソルの座標</param>
	/// <param name="checkPos">押せるスプライトの座標</param>
	/// <param name="radX">スプライトの横の範囲</param>
	/// <param name="radY">スプライトの縦の範囲</param>
	/// <param name="CheckFlag">範囲内に入っている場合フラグをtrueにする</param>
	void CheckCursorIn(const XMFLOAT2& cursorPos, const XMFLOAT2& checkPos, float radX, float radY,bool& CheckFlag);

	/// <summary>
	/// 矢印のスプライトの範囲
	/// </summary>
	/// <param name="cursorPos">カーソルの座標</param>
	/// <param name="checkPos">押せるスプライトの座標</param>
	/// <param name="radX">スプライトの横の範囲</param>
	/// <param name="radY">スプライトの縦の範囲</param>
	/// <returns></returns>
	bool NextorBack(const XMFLOAT2& cursorPos, const XMFLOAT2& checkPos, float radX, float radY);

	/// <summary>
	/// 説明画面で行われる処理
	/// </summary>
	void DescriptionPageProces();

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
	/// 終了処理
	/// </summary>
	void Finalize() override;

private:
	//Obj
	unique_ptr <Object3d> sphere_;
	unique_ptr<Object3d> builshighalpha_[BUILSAMOUNT];
	unique_ptr<Object3d> builslowalpha_[BUILSAMOUNT];
	unique_ptr <Object3d> groundobj_;
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
	unique_ptr<Sprite> arrowrighttrue_ = nullptr;
	unique_ptr<Sprite> arrowlefttrue_ = nullptr;
	//その他の機能
	unique_ptr<Light> light_ = nullptr;
	unique_ptr<LightGroup> lightgroupe_ = nullptr;
	unique_ptr<PostEffect> posteffct_ = nullptr;
	unique_ptr<Audio> clickse_ = nullptr;
	unique_ptr<Audio> morsecodese_ = nullptr;
	unique_ptr<Audio> bgm_ = nullptr;
	unique_ptr<Camera> titlecamera_ = nullptr;
	//球体のステータス
	XMVECTOR spherepos_ = { 0,0,0 };
	XMFLOAT3 spherescl_ = { 4.0f,4.0f,4.0f };
	XMFLOAT3 sphererot_ = { 0,0,0 };
	//フィールドのステータス
	XMVECTOR groundpos_ = { 22.5f,-1,40 };
	XMFLOAT3 groundscl_ = { 13,20,16 };
	XMFLOAT3 groundrot_ = { 0,0,0 };
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
	//説明ページ
	int descriptionpage_ = 0;

	float pointlightpos_[3] = { 0.f,0.f,0.f };
	float pointlightcolor_[3] = { 1.f,1.f,1.f };
	float pointlightatten_[3] = { 0.3f,0.1f,0.1f };


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
};

