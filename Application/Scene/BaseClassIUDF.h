#pragma once
#include"TitleScene.h"
#include"DebugScene.h"
#include"Framework.h"

class BaseClassIUDF:public Framework
{
public:
	/// <summary>
	/// ������
	/// </summary>
	void Initialize() override;
	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;
	/// <summary>
	/// �`��
	/// </summary>
	void Draw() override;
	/// <summary>
	/// �I��
	/// </summary>
	void Finalize() override;
private:
	
	
};

