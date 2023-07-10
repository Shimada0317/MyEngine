#include "Throw.h"

void (Throw::* Throw::StateFuncTable[])() {
	& Throw::AppearanceProcess,
	& Throw::WaitProcess,
	& Throw::AttackProcess,
	& Throw::DeathProcess,
};

Throw::Throw(const XMFLOAT3& allrot, const XMVECTOR& allpos, const XMVECTOR& trackpoint)
{
	body_rot_ = allrot;
	body_rot_.x -= 10;
	base_pos_ = allpos;
	landing_point_ = trackpoint;
	old_hp_ = hp_;
}

void Throw::CreateRobot(Camera* camera)
{
	bringupcamera_ = camera;
	//オブジェクトの生成
	body_ = Object3d::Create(ModelManager::GetInstance()->GetModel(kThrowEnemy));
	propeller_ = Object3d::Create(ModelManager::GetInstance()->GetModel(kEnemyPropeller));
	bullet_ = Object3d::Create(ModelManager::GetInstance()->GetModel(kSphere));

	CommonLoad();

	StatusSet();
}

void Throw::StatusSet()
{
	CommonStatusSet();

	body_pos_ = propeller_pos_ = center_worldpos_;

	body_->SetPosition(body_pos_);
	body_->SetRotation(body_rot_);
	body_->SetScale(body_scl_);

	bullet_->SetPosition(bullet_pos_);
	bullet_->SetRotation(bullet_rot_);
	bullet_->SetScale(bullet_scl_);

	propeller_rot_.y += 15;
	propeller_->SetPosition(propeller_pos_);
	propeller_->SetRotation(propeller_rot_);
	propeller_->SetScale(propeller_scl_);

	rockon_pos_ = Action::GetInstance()->WorldToScreen(center_mat_, body_pos_, bringupcamera_);
	rockon_body_->SetPosition(rockon_pos_);
	rockonother_pos_ = Action::GetInstance()->WorldToScreen(center_mat_, bullet_pos_, bringupcamera_);
	rockon_other_->SetPosition(rockonother_pos_);
}

void Throw::AllUpdate()
{
	body_->Update(color_);
	propeller_->Update(color_);
	bullet_->Update(bullet_color_);

	CommonUpdate();
}

void Throw::Activity(Player* player)
{
	GetPlayerState(player);

	(this->*StateFuncTable[state_])();

	Damage();

	
	ObjParticleDelete();

	AllUpdate();

	StatusSet();

	if (hp_ > 0) { return; }
	state_ = State::kDeath;
}

void Throw::Draw(DirectXCommon* dxCommon)
{
	CommonParticleDraw(dxCommon);
	Object3d::PreDraw(dxCommon->GetCmdList());

	CommonObjDraw();

	body_->Draw();
	propeller_->Draw();
	if (bullet_state_ == BulletState::kShotAfter) {
		bullet_->Draw();
	}
	Object3d::PostDraw();
}

void Throw::AppearanceProcess()
{
}

void Throw::WaitProcess()
{
}

void Throw::AttackProcess()
{
}

void Throw::DeathProcess()
{
}
