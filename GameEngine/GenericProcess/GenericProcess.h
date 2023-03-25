#pragma once
class GenericProcess
{
public:
	/// <summary>
	/// �C���X�^���X����
	/// </summary>
	/// <returns></returns>
	static GenericProcess* GetInstance();

	/// <summary>
	/// int�^�̉����l�A����l�`�F�b�N�֐�
	/// </summary>
	/// <param name="value">��r����l</param>
	/// <param name="low">�����l</param>
	/// <param name="high">����l</param>
	/// <returns></returns>
	int Clamp(int value, int low, int high);

	/// <summary>
	/// float�^�̉����l�A����l�`�F�b�N�֐�
	/// </summary>
	/// <param name="value">��r����l</param>
	/// <param name="low">�����l</param>
	/// <param name="high">����l</param>
	/// <returns></returns>
	float Clampf(float value, float low, float high);

	/// <summary>
	/// ���b�v�A�b�v�A���E���h�֐�(����l�ɒB�����牺���l�ɖ߂��֐�)
	/// </summary>
	/// <param name="value">��r����l</param>
	/// <param name="low">�����l</param>
	/// <param name="high">����l</param>
	/// <returns></returns>
	int Wrap(int value, int low, int high);


};

