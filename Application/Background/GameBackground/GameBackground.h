#pragma once
#include"Object3d.h"
#include"HelperMath.h"
#include<memory>
#include<sstream>

class GameBackground
{
public:
	~GameBackground();
	/// <summary>
	/// CSVの読み込み
	/// </summary>
	void LoadBackgrounndPopData();
	/// <summary>
	/// 背景オブジェクトの生成
	/// </summary>
	void UpdateBackgroudPopCommands();
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();
	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();
private:
	list<unique_ptr<Object3d>> backgrouhnd_;
	stringstream backgrouhnd_popcommands_;
	bool waitflag_ = false;
	int waittime_ = 0;
};

