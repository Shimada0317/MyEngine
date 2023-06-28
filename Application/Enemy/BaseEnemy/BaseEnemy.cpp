//#include "BaseEnemy.h"
//#include"Action.h"
//#include"HelperMath.h"
//
//
//void BaseEnemy::Damage()
//{
//	if (old_hp_ <= base_hp_) { return; }
//	old_hp_ = base_hp_;
//	HitColor();
//	for (int i = 0; i < 5; i++) {
//		std::unique_ptr<ObjParticle> newparticle = std::make_unique<ObjParticle>();
//		newparticle->Initialize(1, base_pos_, { 0.3f,0.3f,0.3f }, { base_rot_ });
//		obj_particle_.push_back(std::move(newparticle));
//	}
//}
//
//
//void BaseEnemy::Death()
//{
//	
//}
//
//bool BaseEnemy::CheckHit(bool& playershot, XMFLOAT2 playerpos, XMFLOAT2 rockonpos, float distance, float mulvalue)
//{
//	//”­–C‚µ‚Ä‚¢‚È‚¢
//	if (!CheckShot(playershot)) { return false; }
//	//À•W‚ªd‚È‚Á‚Ä‚¢‚È‚¢
//	if (!Collision::GetInstance()->CheckHit2D(playerpos, rockonpos, distance, mulvalue)) { return false; }
//	return true;
//}
//
//void BaseEnemy::ParticleEfect()
//{
//	for (int i = 0; i < 50; i++) {
//		XMFLOAT3 pos;
//		pos = HelperMath::GetInstance()->ConvertToXMVECTOR(base_pos_);
//
//		const float rnd_vel = 0.04f;
//		XMFLOAT3 Vel{};
//		XMFLOAT3 MinValue{ -0.09f,-0.01f,-0.03f };
//		XMFLOAT3 MaxValue{ 0.09f,0.12f,0.09f };
//		Vel = Action::GetInstance()->RandMinAndMax(MinValue, MaxValue);
//
//		XMFLOAT3 acc{};
//		acc.y = 0.0;
//
//		partred_->Add(200, pos, Vel, acc, 4.0f, 0.0f, 150.0f);
//		partgreen_->Add(200, pos, Vel, acc, 3.7f, 0.0f, 150.0f);
//	}
//}
//
//bool BaseEnemy::CheckShot(bool playershot)
//{
//	if (!playershot) { return false; }
//	return true;
//	
//
//}
