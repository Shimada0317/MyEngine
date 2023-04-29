#pragma once
#include"Object3d.h"
#include"HelperMath.h"

const int BUILSAMOUNT = 16;

class CommonBackground
{
public:
	~CommonBackground();

	void Initialize();

	void Update();

	void Draw();
private:
	unique_ptr<Object3d> buils_front_[BUILSAMOUNT];
	unique_ptr<Object3d> buils_back_[BUILSAMOUNT];
	unique_ptr<Object3d> sphere_;
	unique_ptr<Object3d> ground_;
	unique_ptr<Object3d> start_;
	

};

