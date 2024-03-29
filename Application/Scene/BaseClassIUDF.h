#pragma once
#include"TitleScene.h"
#include"DebugScene.h"
#include"Framework.h"

class BaseClassIUDF:public Framework
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
	
	
};

