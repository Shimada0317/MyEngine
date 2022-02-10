#pragma once
#include"Framework.h"

class MyGame:public Framework
{
public:
	/// <summary>
	/// ‰Šú‰»
	/// </summary>
	void Initialize() override;
	/// <summary>
	/// XV
	/// </summary>
	void Update() override;
	/// <summary>
	/// •`‰æ
	/// </summary>
	void Draw() override;
	/// <summary>
	/// I—¹
	/// </summary>
	void Finalize() override;

	
};

