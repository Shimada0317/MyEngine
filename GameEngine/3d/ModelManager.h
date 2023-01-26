#pragma once
#include"ObjModel.h"
#include<vector>

using namespace std;

class ModelManager final
{
private:
	//�f�X�g���N�^��private�ɂ���
	~ModelManager();
	//�R���X�g���N�^��private�ɂ���
	ModelManager()=default;
	
public:
	//�R�s�[�R���X�g���N�^�𖳌��ɂ���
	ModelManager(const ModelManager& model) = delete;
	//������Z�q�𖳌��ɂ���
	ModelManager& operator=(const ModelManager& model) = delete;

	static ModelManager* GetInstance();

	void Initialize();

	void CallModel(const std::string& ObjModelname);

	ObjModel* GetModel(int number) { return model_[number]; }
private:
	vector<ObjModel*> model_;
	vector<int> contena;
};

