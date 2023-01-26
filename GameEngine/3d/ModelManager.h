#pragma once
#include"ObjModel.h"
#include<vector>

using namespace std;

class ModelManager final
{
private:
	//デストラクタをprivateにする
	~ModelManager();
	//コンストラクタをprivateにする
	ModelManager()=default;
	
public:
	//コピーコンストラクタを無効にする
	ModelManager(const ModelManager& model) = delete;
	//代入演算子を無効にする
	ModelManager& operator=(const ModelManager& model) = delete;

	static ModelManager* GetInstance();

	void Initialize();

	void CallModel(const std::string& ObjModelname);

	ObjModel* GetModel(int number) { return model_[number]; }
private:
	vector<ObjModel*> model_;
	vector<int> contena;
};

