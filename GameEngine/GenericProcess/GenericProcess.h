#pragma once
class GenericProcess
{
public:
	/// <summary>
	/// インスタンス生成
	/// </summary>
	/// <returns></returns>
	static GenericProcess* GetInstance();

	/// <summary>
	/// int型の下限値、上限値チェック関数
	/// </summary>
	/// <param name="value">比較する値</param>
	/// <param name="low">下限値</param>
	/// <param name="high">上限値</param>
	/// <returns></returns>
	int Clamp(int value, int low, int high);

	/// <summary>
	/// float型の下限値、上限値チェック関数
	/// </summary>
	/// <param name="value">比較する値</param>
	/// <param name="low">下限値</param>
	/// <param name="high">上限値</param>
	/// <returns></returns>
	float Clampf(float value, float low, float high);

	/// <summary>
	/// ラップアップアラウンド関数(上限値に達したら下限値に戻す関数)
	/// </summary>
	/// <param name="value">比較する値</param>
	/// <param name="low">下限値</param>
	/// <param name="high">上限値</param>
	/// <returns></returns>
	int Wrap(int value, int low, int high);


};

