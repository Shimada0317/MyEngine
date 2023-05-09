#pragma once
#include"ObjModel.h"
#include<vector>

using namespace std;

enum ModelName {
	kWorm = 0,
	kGear,
	kShadow,
	kHead,
	kBody,
	kArms,
	kSkydome,
	kBuils,
	kBuil,
	kGround,
	kGun,
	kHeli,
	kHane,
	kSphere,
	kThrowEnemy,
	kEnemyPropeller,
};

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

	void CallModel(const std::string& ObjModelname,bool Smoothing=false);

	ObjModel* GetModel(int number) { return model_[number]; }
private:
	vector<ObjModel*> model_;
	vector<int> contena;
};