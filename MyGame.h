#pragma once
#include"Framework.h"

class GameScene;

class MyGame:public Framework
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;
	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;
	/// <summary>
	/// 終了
	/// </summary>
	void Finalize() override;
private:
	//シーン
	GameScene* scene_;
	
};

