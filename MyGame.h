#pragma once
#include"Framework.h"

class MyGame:public Framework
{
public:
	/// <summary>
	/// ������
	/// </summary>
	void Initialize();
	/// <summary>
	/// �X�V
	/// </summary>
	void Update();
	/// <summary>
	/// �`��
	/// </summary>
	void Draw();
	/// <summary>
	/// �I��
	/// </summary>
	void Finalize();

	
};

