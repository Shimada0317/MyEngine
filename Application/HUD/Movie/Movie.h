#pragma once
#include"HelperMath.h"
#include"Sprite.h"

class Movie
{
public:

	~Movie();
	/// <summary>
	/// ����
	/// </summary>
	void Create();
	/// <summary>
	/// �X�e�[�^�X�̃Z�b�g
	/// </summary>
	void StatusSet();
	/// <summary>
	/// �\��
	/// </summary>
	void Disply();
	/// <summary>
	/// ��\��
	/// </summary>
	void Invisible();
	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

private:
	unique_ptr<Sprite> curtainup_;
	unique_ptr<Sprite> curtaindown_;
	unique_ptr<Sprite> skip_;

	//�n�܂�ƏI���̉��o�g�������J�[�e��
	XMFLOAT2 curtainuppos_ = { 0.0f,0.0f };
	XMFLOAT2 curtainsize_ = { 1280.0f,100.0f };
	XMFLOAT2 curtaindownpos_ = { 0.0f,620.0f };
	//Skip�����̕\��
	XMFLOAT2 skippos_ = { 0.0f,620.0f };
};

