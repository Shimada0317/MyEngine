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
	/// CSV�̓ǂݍ���
	/// </summary>
	void LoadBackgrounndPopData();
	/// <summary>
	/// �w�i�I�u�W�F�N�g�̐���
	/// </summary>
	void UpdateBackgroudPopCommands();
	/// <summary>
	/// �X�V����
	/// </summary>
	void Update();
	/// <summary>
	/// �`�揈��
	/// </summary>
	void Draw();
private:
	list<unique_ptr<Object3d>> backgrouhnd_;
	stringstream backgrouhnd_popcommands_;
	bool waitflag_ = false;
	int waittime_ = 0;
};

