#pragma once
#include"BaseScene.h"
#include"DirectXCommon.h"
/// <summary>
/// シーン管理
/// </summary>
class SceneManager
{
public:
	~SceneManager();

	/// <summary>
	/// 更新
	/// </summary>
	void Update(DirectXCommon* dxComon);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(DirectXCommon* dxCommon);

	//次シーンの予約
	void SetNextScene(BaseScene* nextScene) { nextscene_ = nextScene; }
private:
	//今のシーン
	BaseScene* scene_ = nullptr;
	//次のシーン
	BaseScene* nextscene_ = nullptr;
};

