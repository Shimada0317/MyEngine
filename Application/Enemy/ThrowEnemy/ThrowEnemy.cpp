#include "ThrowEnemy.h"
#include"Action.h"
#include"HelperMath.h"
#include"ModelManager.h"
#include"SpriteManager.h"

using namespace DirectX;

const int BodyDamage = 25;
const float FallSpeed = 0.15f;
const XMFLOAT4 AddColor = { 0.1f,0.1f,0.1f,0.0f };


ThrowEnemy::~ThrowEnemy()
{
	enemy_.reset();
	propeller_.reset();
}
//初期化処理
void ThrowEnemy::Initialize(const XMFLOAT3& allrot, const XMVECTOR& allpos, Camera* camera, const XMVECTOR& trackpos)
{
	HaveCamera = camera;
	//オブジェクトの生成
	enemy_ = Object3d::Create(ModelManager::GetInstance()->GetModel(kThrowEnemy));
	propeller_ = Object3d::Create(ModelManager::GetInstance()->GetModel(kEnemyPropeller));
	Center = Object3d::Create(ModelManager::GetInstance()->GetModel(kShadow));
	bullet_ = Object3d::Create(ModelManager::GetInstance()->GetModel(kSphere));
	//スプライトの生成
	RockOn.reset(Sprite::SpriteCreate(Name::kEnemyMarker, RockOnPos, RockOnCol, RockOnAnchorPoint));
	rockon_bullet_.reset(Sprite::SpriteCreate(Name::kEnemyMarker, rockon_bulletpos_, RockOnCol, RockOnAnchorPoint));
	
	PartGreen = ParticleManager::Create(HaveCamera);
	PartRed = ParticleManager::Create(HaveCamera);
	floating_pos_ = Action::GetInstance()->GetRangRand(6.f, 7.f);
	body_rot_ = allrot;
	body_rot_.x -= 10;
	center_pos_ = allpos;
	CenterMat = Center->GetMatrix();
	CenterWorldPos = XMVector3TransformNormal(center_pos_, CenterMat);
	LandingPoint = trackpos;
	OldHp = Hp;
	Center->SetPosition(CenterWorldPos);
}
//ステータスのセット
void ThrowEnemy::StatusSet()
{

	Center->SetScale({ 1.f,1.f,1.f });
	XMMatrixIsIdentity(CenterMat);
	CenterMat = Center->GetMatrix();
	CenterWorldPos = XMVector3TransformNormal(center_pos_, CenterMat);
	Center->SetPosition(CenterWorldPos);

	body_pos_ = propeller_pos_ = CenterWorldPos;

	enemy_->SetPosition(body_pos_);
	enemy_->SetRotation(body_rot_);
	enemy_->SetScale(body_scl_);
	
	bullet_->SetPosition(bullet_pos_);
	bullet_->SetRotation(bullet_rot_);
	bullet_->SetScale(bullet_scl_);

	propeller_rot_.y += 15;
	propeller_->SetPosition(propeller_pos_);
	propeller_->SetRotation(propeller_rot_);
	propeller_->SetScale(propeller_scl_);

	RockOnPos = WorldtoScreen(body_pos_);
	RockOn->SetPosition(RockOnPos);
	rockon_bulletpos_ = WorldtoScreen(bullet_pos_);
	rockon_bullet_->SetPosition(rockon_bulletpos_);
}
//全ての更新処理をまとめる
void ThrowEnemy::AllUpdate()
{
	enemy_->Update();
	propeller_->Update();
	Center->Update();
	bullet_->Update();

	PartRed->Update({ 1.0f,0.0f,0.0f,0.0f });
	PartGreen->Update({ 0.0f,0.5f,0,0.0f });
	for (unique_ptr<ObjParticle>& patrticle : obj_particle_) {
		patrticle->Update();
	}
}
//更新処理
void ThrowEnemy::Update(const XMFLOAT2& player2Dpos, int& playerhp, bool& playerbulletshot)
{
	obj_particle_.remove_if([](std::unique_ptr<ObjParticle>& particle) {
		return particle->IsDelete();
		});
	//登場処理
	AppearanceProcess();
	//待機処理
	WaitProcess();
	//攻撃処理
	AttackProcess(player2Dpos,playerhp,playerbulletshot);
	//ダメージを食らったときの処理
	DamageProcess(player2Dpos,playerbulletshot);
	//死亡処理
	DeathProcess();
	if (Input::GetInstance()->PushKey(DIK_O)) {
		Hp = 0;
	}
	if (Hp <= 0) {
		state_ = State::DEATH;
	}
	//ステータスのセット
	StatusSet();
	//全ての更新処理をまとめる
	AllUpdate();
}
//登場処理
void ThrowEnemy::AppearanceProcess()
{
	if (state_ != State::APPEARANCE) { return; }
	//落下してくる
	center_pos_.m128_f32[1] -= FallSpeed;
	if (center_pos_.m128_f32[1] <= floating_pos_) {
		bullet_pos_ = CenterWorldPos;
		bullet_pos_.m128_f32[1] =bullet_pos_.m128_f32[1] - 1.f;
		old_pos_ = bullet_pos_;
		state_ = State::WAIT;
	}

}
//待機処理
void ThrowEnemy::WaitProcess()
{
	if (state_ != State::WAIT) { return; }
	bullet_active_ = true;
	bullet_scl_=HelperMath::GetInstance()->XMFLOAT3AddFloat(bullet_scl_, 0.001f);
	if (bullet_scl_.z <= 0.3f) { return; }
	state_ = State::ATTACK;
}
//攻撃処理
void ThrowEnemy::AttackProcess(const XMFLOAT2& player2Dpos, int& playerhp, bool& playerbulletshot)
{
	if (state_ != State::ATTACK) { return; }
	//弾発射
	ThrowAttack(playerhp);
	//弾の当たり判定
	BulletCollision(player2Dpos, playerbulletshot);
}
//ダメージを食らったときの処理
void ThrowEnemy::DamageProcess(const XMFLOAT2& player2Dpos, bool& playerbulletshot)
{
	float vx = 0;
	float vy = 0;
	float vz = 0;

	vx = (bullet_pos_.m128_f32[0] - LandingPoint.m128_f32[0]);
	vy = (bullet_pos_.m128_f32[1] - LandingPoint.m128_f32[1] - 2);
	vz = (bullet_pos_.m128_f32[2] - LandingPoint.m128_f32[2]);

	float v2x = powf(vx, 2.f);
	float v2y = powf(vy, 2.f);
	float v2z = powf(vz, 2.f);

	Length = sqrtf(v2x + v2y + v2z);
	Distance = Length;

	//当たり判定
	if (playerbulletshot == true && Hp > 0) {
		if (player2Dpos.x - Distance * 2.f < RockOnPos.x && player2Dpos.x + Distance * 2.f > RockOnPos.x &&
			player2Dpos.y - Distance * 2.f < RockOnPos.y && player2Dpos.y + Distance * 2.f > RockOnPos.y) {
			Hp -= BodyDamage;
			playerbulletshot = false;
		}
	}
	if (Hp >= OldHp) { return; }
	for (int i = 0; i < 5; i++) {
		std::unique_ptr<ObjParticle> newparticle = std::make_unique<ObjParticle>();
		newparticle->Initialize(1, body_pos_, { 0.3f,0.3f,0.3f }, { body_rot_ });
		obj_particle_.push_back(std::move(newparticle));
	}
	OldHp = Hp;
}
//死亡処理
void ThrowEnemy::DeathProcess()
{
	if (state_ != State::DEATH) { return; }
	float gravity_ = 1.8f;
	fall_time_ += 0.001f;
	float fallspeed = gravity_ * fall_time_;
	if (center_pos_.m128_f32[1] >= 0) {
		center_pos_.m128_f32[1] -= fallspeed;
		if (center_pos_.m128_f32[1] <= 0) {
			DeadFlag = true;
		}
	}
	if (particle_flag_ != true) { return; }
	ParticleEfect();
}
//3Dから2Dに変換
XMFLOAT2 ThrowEnemy::WorldtoScreen(const XMVECTOR& set3Dposition)
{
	Center->SetRotation(CenterRot);
	CenterMat = Center->GetMatrix();
	const float kDistancePlayerTo3DReticle = 50.0f;
	offset = { 0.0,0.0,1.0f };
	offset = XMVector3TransformNormal(offset, CenterMat);
	offset = XMVector3Normalize(offset) * kDistancePlayerTo3DReticle;

	XMVECTOR PositionRet = set3Dposition;

	HelperMath::GetInstance()->ChangeViewPort(MatViewPort, offset);

	XMMATRIX MatVP = MatViewPort;

	XMMATRIX View = HaveCamera->GetViewMatrix();
	XMMATRIX Pro = HaveCamera->GetProjectionMatrix();

	XMMATRIX MatViewProjectionViewport = View * Pro * MatVP;

	PositionRet = XMVector3TransformCoord(PositionRet, MatViewProjectionViewport);

	XMFLOAT2 get2dposition;
	get2dposition.x = PositionRet.m128_f32[0];
	get2dposition.y = PositionRet.m128_f32[1];

	return get2dposition;
}
//描画処理
void ThrowEnemy::Draw(DirectXCommon* dxCommon)
{
	

	//オブジェクト
	Object3d::PreDraw(dxCommon->GetCmdList());
	for (std::unique_ptr<ObjParticle>& particle : obj_particle_) {
		particle->Draw();
	}
	enemy_->Draw();
	propeller_->Draw();
	if (bullet_active_==true) {
		bullet_->Draw();
	}
	Object3d::PostDraw();

	ParticleManager::PreDraw(dxCommon->GetCmdList());
	PartGreen->Draw();
	PartRed->Draw();
	ParticleManager::PostDraw();
	//スプライト
	Sprite::PreDraw(dxCommon->GetCmdList());
	RockOn->Draw();
	rockon_bullet_->Draw();
	Sprite::PostDraw();
}
//弾の当たり判定
void ThrowEnemy::BulletCollision(const XMFLOAT2& player2Dpos, bool& playerbulletshot)
{
	//当たり判定
	if (playerbulletshot == false) { return; }
	if (player2Dpos.x - bullet_distance_ * 4.f < rockon_bulletpos_.x && player2Dpos.x + bullet_distance_ * 4.f > rockon_bulletpos_.x &&
		player2Dpos.y - bullet_distance_ * 4.f < rockon_bulletpos_.y && player2Dpos.y + bullet_distance_ * 4.f > rockon_bulletpos_.y) {
		bullet_active_ = false;
		playerbulletshot = false;
	}
}
//弾発射
void ThrowEnemy::ThrowAttack(int& playerhp)
{
	float vx = 0;
	float vy = 0;
	float vz = 0;
	float bullet_length = 0;

	vx = (bullet_pos_.m128_f32[0] - LandingPoint.m128_f32[0]);
	vy = (bullet_pos_.m128_f32[1] - LandingPoint.m128_f32[1]-2);
	vz = (bullet_pos_.m128_f32[2] - LandingPoint.m128_f32[2]);

	float v2x = powf(vx, 2.f);
	float v2y = powf(vy, 2.f);
	float v2z = powf(vz, 2.f);

	bullet_length = sqrtf(v2x + v2y + v2z);

	XMVECTOR v3;
	v3.m128_f32[0] = (vx / bullet_length) * bullet_speed_;
	v3.m128_f32[1] = (vy / bullet_length) * bullet_speed_;
	v3.m128_f32[2] = (vz / bullet_length) * bullet_speed_;

	
	bullet_distance_ = bullet_length;

	bullet_pos_ -= v3;
	if (bullet_length <=0.1f) {
		playerhp -= 1;
		bullet_pos_ = old_pos_;
		bullet_scl_={};
		state_ = State::WAIT;
	}

	if (bullet_active_ == false) {
		bullet_pos_ = old_pos_;
		bullet_scl_ = {};
		state_ = State::WAIT;
	}
}

void ThrowEnemy::ParticleEfect()
{
	for (int i = 0; i < 10; i++) {
		XMFLOAT3 pos;
		pos.x = CenterWorldPos.m128_f32[0];
		pos.y = CenterWorldPos.m128_f32[1]-1.f;
		pos.z = CenterWorldPos.m128_f32[2];

		const float rnd_vel = 0.04f;
		XMFLOAT3 vel{};
		vel.x = Action::GetInstance()->GetRangRand(-0.09f, 0.09f);
		vel.y = Action::GetInstance()->GetRangRand(-0.11f, 0.12f);
		vel.z = Action::GetInstance()->GetRangRand(-0.09f, 0.09f);

		XMFLOAT3 acc{};
		acc.y = 0.0;

		PartRed->Add(30, pos, vel, acc, 1.0f, 0.0f, 0.0f);
		PartGreen->Add(30, pos, vel, acc, 0.7f, 0.0f, 0.0f);
	}
	//particle_flag_ = false;
}
