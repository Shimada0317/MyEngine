#pragma once


class ModelManager final
{
private:
	//コンストラクタをprivateにする
	ModelManager();
	//デストラクタをprivateにする
	~ModelManager();
public:
	//コピーコンストラクタを無効にする
	ModelManager(const ModelManager& model) = delete;
	//代入演算子を無効にする
	ModelManager& operator=(const ModelManager& model) = delete;

	static ModelManager* GetInstance();
};

