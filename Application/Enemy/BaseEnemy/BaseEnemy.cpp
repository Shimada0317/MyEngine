#include "BaseEnemy.h"
#include"Player.h"
#include"SpriteManager.h"


void BaseEnemy::Update(Player* player)
{
	Activity(player);
}

void BaseEnemy::CommonLoad()
{
	partgreen_ = ParticleManager::Create(bringupcamera_);
	partred_ = ParticleManager::Create(bringupcamera_);
	center_mat_ = center_->GetMatrix();
	center_worldpos_ = XMVector3TransformNormal(base_pos_, center_mat_);
	center_->SetPosition(center_worldpos_);
	rockon_body_.reset(Sprite::SpriteCreate(Name::kEnemyMarker, rockon_pos_, rockon_color_, anchorpoint_));
	rockon_other_.reset(Sprite::SpriteCreate(Name::kEnemyMarker, rockonother_pos_, rockon_color_, anchorpoint_));
}

void BaseEnemy::CommonStatusSet()
{
	center_->SetScale({ 1.0f,1.0f,1.0f });
	XMMatrixIsIdentity(center_mat_);
	center_mat_ = center_->GetMatrix();
	center_worldpos_ = XMVector3TransformNormal(base_pos_, center_mat_);
	center_->SetPosition(center_worldpos_);

	shadow_pos_ = center_worldpos_;
	shadow_pos_.m128_f32[1] = -0.8f;
	shadow_->SetPosition(shadow_pos_);
	shadow_->SetRotation({ 0.0f,0.0f,0.0f });
	shadow_->SetScale({ 1.0f,1.0f,1.0f });
}

void BaseEnemy::CommonUpdate()
{
	shadow_->Update(shadow_color_);
	center_->Update();

	partred_->Update({ 1.0f,0.0f,0.0f,0.0f });
	partgreen_->Update({ 0.0f,0.5f,0,0.0f });

	for (std::unique_ptr<ObjParticle>& patrticle : obj_particle_) {
		patrticle->Update();
	}
}

void BaseEnemy::GetPlayerState(Player* player)
{
	player_ = player;
	player_pos_ = player_->GetRetPosition();
	player_shot_ = player_->GetBulletShot();
	player_hp_ = player_->GetHp();
}

void BaseEnemy::ObjParticleDelete()
{
	obj_particle_.remove_if([](std::unique_ptr<ObjParticle>& particle) {
		return particle->IsDelete();
		});
}

void BaseEnemy::CommonCollision()
{
	if (Collision::GetInstance()->CheckHit2D(player_pos_, rockon_pos_, distance_, 1.3f)) {
		hp_ -= damage_value_;
		player_shot_ = false;
	}
}

void BaseEnemy::TrackCalculation()
{
	//’Ç”ö‚ÌŒvŽZ
	XMFLOAT3 Value;
	Value = HelperMath::GetInstance()->TrackCalculation(base_pos_, track_point_);
	//’l‚ð2æ
	XMFLOAT3 SquareValue{};
	SquareValue = HelperMath::GetInstance()->SquareToXMFLOAT3(Value, 2);
	//‹——£‚ÌŒvŽZ
	length_ = HelperMath::GetInstance()->LengthCalculation(SquareValue);
	//’Ç”ö‘¬“x‚ÌŒvŽZ
	TrackSpeed={};
	TrackSpeed = HelperMath::GetInstance()->TrackingVelocityCalculation(Value, length_, speed_);
}


void BaseEnemy::CommonParticleDraw(DirectXCommon* dxCommon)
{
	ParticleManager::PreDraw(dxCommon->GetCmdList());
	partgreen_->Draw();
	partred_->Draw();
	ParticleManager::PostDraw();
}

void BaseEnemy::CommonObjDraw()
{
	for (std::unique_ptr<ObjParticle>& particle : obj_particle_) {
		particle->Draw();
	}
}

void BaseEnemy::ParticleEffect()
{
	for (int i = 0; i < 50; i++) {
		XMFLOAT3 pos;
		pos = HelperMath::GetInstance()->ConvertToXMVECTOR(base_pos_);

		const float rnd_vel = 0.04f;
		XMFLOAT3 Vel{};
		XMFLOAT3 MinValue{ -0.09f,-0.01f,-0.03f };
		XMFLOAT3 MaxValue{ 0.09f,0.12f,0.09f };
		Vel = Action::GetInstance()->RandMinAndMax(MinValue, MaxValue);

		XMFLOAT3 acc{};
		acc.y = 0.0;

		partred_->Add(200, pos, Vel, acc, 4.0f, 0.0f, 150.0f);
		partgreen_->Add(200, pos, Vel, acc, 3.7f, 0.0f, 150.0f);
	}
}

void BaseEnemy::Damage()
{
	if (old_hp_ <= hp_) { return; }
	old_hp_ = hp_;
	for (int i = 0; i < 5; i++) {
		std::unique_ptr<ObjParticle> newparticle = std::make_unique<ObjParticle>();
		newparticle->Initialize(1, base_pos_, { 0.3f,0.3f,0.3f }, { base_rot_ });
		obj_particle_.push_back(std::move(newparticle));
	}

}
