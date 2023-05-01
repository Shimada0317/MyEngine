#include "CommonBackground.h"
#include"ModelManager.h"

CommonBackground::~CommonBackground()
{
	sphere_.reset();
	start_.reset();
	ground_.reset();
	for (int i = 0; i < BUILSAMOUNT; i++) {
		buils_front_[i].reset();
		buils_front_[i].reset();
	}
}

void CommonBackground::Initialize()
{
	//�v���C���[���ŏ��ɂ��錚��
	const XMVECTOR start_pos_ = { 0.0f,0.0f,-16.5f };
	const XMFLOAT3 start_scl_ = { 15.0f,15.0f,15.0f };
	const XMFLOAT3 start_rot_ = { 0.0f,180.0f,0.0f };
	//�r���Q�̃X�e�[�^�X
	const XMFLOAT3 buils_scl_ = { 10.0f,10.0f,10.0f };
	XMFLOAT3 buils_rot_ = { 0.0f,90.0f,0.0f };
	XMVECTOR buils_frontpos_ = { 0.0f,0.0f,-16.5f };
	XMVECTOR buils_backpos_ = { 0.0f,0.0f,-16.5f };
	//����̃X�e�[�^�X
	const XMVECTOR ground_pos_ = { 0.0f,-1.1f,0.0f };
	const XMFLOAT3 ground_scl_ = { 100,100,100 };
	//�w�i�̓V���̃X�e�[�^�X
	const XMVECTOR sphere_pos_ = { 0,0,0 };
	const XMFLOAT3 sphere_scl_ = { 4.0f,4.0f,4.0f };
	const XMFLOAT3 sphere_rot_ = { 0,0,0 };
	//�I�u�W�F�N�g�̐���
	sphere_ = Object3d::Create(ModelManager::GetInstance()->GetModel(kSkydome));
	start_ = Object3d::Create(ModelManager::GetInstance()->GetModel(kBuil));
	ground_ = Object3d::Create(ModelManager::GetInstance()->GetModel(kGround));

	for (int i = 0; i < BUILSAMOUNT; i++) {
		buils_front_[i] = Object3d::Create(ModelManager::GetInstance()->GetModel(kBuils));
		buils_back_[i] = Object3d::Create(ModelManager::GetInstance()->GetModel(kBuils));
		buils_front_[i]->SetScale(buils_scl_);
		buils_back_[i]->SetScale(buils_scl_);
		//��ʂ̉E���ɔz�u
		if (i % 2 == 0) {
			buils_frontpos_ = { 100.0f, 0,-300.0f + (100 * i / 2) };
			buils_backpos_ = { 200.0f,0,-300.0f + (100 * i / 2) };
			buils_rot_ = { 0.0f,90.0f,0.0f };
		}
		//��ʂ̍����ɔz�u
		else if (i % 2 == 1) {
			buils_frontpos_ = { -100.0f,0,-300.0f + (100 * i / 2) };
			buils_backpos_ = { -200.0f, 0,-300.0f + (100 * i / 2) };
			buils_rot_ = { 0.0f,270.0f,0.0f };
		}
		buils_front_[i]->SetRotation(buils_rot_);
		buils_back_[i]->SetRotation(buils_rot_);
		buils_front_[i]->SetPosition(buils_frontpos_);
		buils_back_[i]->SetPosition(buils_backpos_);
	}
	//���W�̃Z�b�g
	sphere_->SetPosition(sphere_pos_);
	sphere_->SetRotation(sphere_rot_);
	sphere_->SetScale(sphere_scl_);
	//�n�ʂ̃X�e�[�^�X�̃Z�b�g
	ground_->SetPosition(ground_pos_);
	ground_->SetScale(ground_scl_);
	//�J�����̈ړ���̃r���̃X�e�[�^�X�Z�b�g
	start_->SetPosition(start_pos_);
	start_->SetScale(start_scl_);
	start_->SetRotation(start_rot_);
}

void CommonBackground::Update()
{
	//�e�I�u�W�F�N�g�̐F
	const XMFLOAT4 bill_color_ = { 0.8f,0.6f,0.3f,1.0f };
	const XMFLOAT4 sphere_color_ = { 1.f,1.f,1.f,1.f };
	//���E�̃r���̍X�V����
	for (int i = 0; i < BUILSAMOUNT; i++) {
		buils_front_[i]->Update(bill_color_);
		buils_back_[i]->Update(bill_color_);
	}
	//�V���̍X�V����
	sphere_->Update(sphere_color_, true);
	//�n�ʂ̍X�V����
	ground_->Update();
	//�J�����̈ړ���̃r���̍X�V����
	start_->Update(bill_color_);
}

void CommonBackground::Draw()
{
	ground_->Draw();
	sphere_->Draw();
	start_->Draw();
	for (int i = 0; i < BUILSAMOUNT; i++) {
		buils_front_[i]->Draw();
		buils_back_[i]->Draw();
	}
}
