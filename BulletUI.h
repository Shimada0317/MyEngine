#pragma once
#include"HelperMath.h"
#include"Sprite.h"
#include"WinApp.h"

class BulletUI
{
public:
	~BulletUI();
	/// <summary>
	/// ����
	/// </summary>
	/// <param name="remaining">�c�e��</param>
	/// <param name="bulletuipos">�e��UI�����W</param>
	/// <param name="reloadpos">�����[�h�̕����̍��W</param>
	void Create(int remaining,const XMFLOAT2& bulletuipos,const XMFLOAT2& reloadpos);
	/// <summary>
	/// �X�e�[�^�X�̃Z�b�g
	/// </summary>
	void Set();
	/// <summary>
	/// �e�𔭖C�����Ƃ��̏���
	/// </summary>
	/// <param name="remaining">�c�e��</param>
	void Shot(int remaining);
	/// <summary>
	/// �e�̃Z�b�g
	/// </summary>
	/// <param name="remaining">�c�e��</param>
	void SetRemainig(int remaining) { this->remaining_ = remaining; };
	/// <summary>
	/// �����[�h�̏���
	/// </summary>
	/// <param name="remaining">�c�e��</param>
	void Reload(int  remaining);
	/// <summary>
	/// �����Ă�������
	/// </summary>
	void FallingUI();
	/// <summary>
	/// �����[�h�̕�������
	/// </summary>
	void ReloadMotion();
	/// <summary>
	/// �`��
	/// </summary>
	void Draw();
private:
	//Sprite
	unique_ptr<Sprite> bullet_ui_[9];
	unique_ptr<Sprite> reload_ui_;
	//�e��UI�̃X�e�[�^�X
	XMFLOAT2 bullet_pos_[9];
	XMFLOAT2 original_bulletpos_;
	float bullet_rot_[9];
	float time_[9];
	bool drop_bulletflag_[9];
	//Reload�̃X�e�[�^�X
	XMFLOAT4 reload_color_{ 1.0f,1.0f,1.0f,0.9f };
	XMFLOAT2 reload_pos_{ WinApp::window_width / 2,210.f };
	XMFLOAT2 reload_size_{ 210.f,140.f };
	XMFLOAT2 reload_oldsize_ = reload_size_;
	//�C�[�W���O�p�̃t���O�ƃ^�C�}�[
	float reload_time_{};
	bool easing_changeflag_ = false;
	//�c�e���̉ߋ��̎c�e��
	int remaining_{};
	int old_remaining_{};
};

