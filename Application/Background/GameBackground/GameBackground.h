#pragma once
#include"Object3d.h"
#include"HelperMath.h"
#include<memory>
#include<sstream>

class GameBackground
{
public:
	~GameBackground();

	void LoadBackgrounndPopData();

	void UpdateBackgroudPopCommands();

	void Update();

	void Draw();
private:
	list<unique_ptr<Object3d>> backgrouhnd_;
	stringstream backgrouhnd_popcommands_;
	bool waitflag_ = false;
	int waittime_ = 0;
};

