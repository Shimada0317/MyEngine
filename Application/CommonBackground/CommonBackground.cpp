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
	//プレイヤーが最初にいる建物
	const XMVECTOR start_pos_ = { 0.0f,0.0f,-16.5f };
	const XMFLOAT3 start_scl_ = { 15.0f,15.0f,15.0f };
	const XMFLOAT3 start_rot_ = { 0.0f,180.0f,0.0f };
	//ビル群のステータス
	const XMFLOAT3 buils_scl_ = { 10.0f,10.0f,10.0f };
	XMFLOAT3 buils_rot_ = { 0.0f,90.0f,0.0f };
	XMVECTOR buils_frontpos_ = { 0.0f,0.0f,-16.5f };
	XMVECTOR buils_backpos_ = { 0.0f,0.0f,-16.5f };
	//足場のステータス
	const XMVECTOR ground_pos_ = { 0.0f,-1.1f,0.0f };
	const XMFLOAT3 ground_scl_ = { 100,100,100 };
	//背景の天球のステータス
	const XMVECTOR sphere_pos_ = { 0,0,0 };
	const XMFLOAT3 sphere_scl_ = { 4.0f,4.0f,4.0f };
	const XMFLOAT3 sphere_rot_ = { 0,0,0 };
	//オブジェクトの生成
	sphere_ = Object3d::Create(ModelManager::GetInstance()->GetModel(kSkydome));
	start_ = Object3d::Create(ModelManager::GetInstance()->GetModel(kBuil));
	ground_ = Object3d::Create(ModelManager::GetInstance()->GetModel(kGround));

	for (int i = 0; i < BUILSAMOUNT; i++) {
		buils_front_[i] = Object3d::Create(ModelManager::GetInstance()->GetModel(kBuils));
		buils_back_[i] = Object3d::Create(ModelManager::GetInstance()->GetModel(kBuils));
		buils_front_[i]->SetScale(buils_scl_);
		buils_back_[i]->SetScale(buils_scl_);
		//画面の右側に配置
		if (i % 2 == 0) {
			buils_frontpos_ = { 100.0f, 0,-300.0f + (100 * i / 2) };
			buils_backpos_ = { 200.0f,0,-300.0f + (100 * i / 2) };
			buils_rot_ = { 0.0f,90.0f,0.0f };
		}
		//画面の左側に配置
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
	//座標のセット
	sphere_->SetPosition(sphere_pos_);
	sphere_->SetRotation(sphere_rot_);
	sphere_->SetScale(sphere_scl_);
	//地面のステータスのセット
	ground_->SetPosition(ground_pos_);
	ground_->SetScale(ground_scl_);
	//カメラの移動先のビルのステータスセット
	start_->SetPosition(start_pos_);
	start_->SetScale(start_scl_);
	start_->SetRotation(start_rot_);
}

void CommonBackground::Update()
{
	//各オブジェクトの色
	const XMFLOAT4 bill_color_ = { 0.8f,0.6f,0.3f,1.0f };
	const XMFLOAT4 sphere_color_ = { 1.f,1.f,1.f,1.f };
	//左右のビルの更新処理
	for (int i = 0; i < BUILSAMOUNT; i++) {
		buils_front_[i]->Update(bill_color_);
		buils_back_[i]->Update(bill_color_);
	}
	//天球の更新処理
	sphere_->Update(sphere_color_, true);
	//地面の更新処理
	ground_->Update();
	//カメラの移動先のビルの更新処理
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
