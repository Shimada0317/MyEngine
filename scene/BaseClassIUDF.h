#pragma once
#include"Framework.h"

class BaseClassIUDF:public Framework
{
public:
	/// <summary>
	/// ‰Šú‰»
	/// </summary>
	void Initialize() override;
	/// <summary>
	/// XV
	/// </summary>
	void Updata() override;
	/// <summary>
	/// •`‰æ
	/// </summary>
	void Draw() override;
	/// <summary>
	/// I—¹
	/// </summary>
	void Finalize() override;
private:
	
	
};

