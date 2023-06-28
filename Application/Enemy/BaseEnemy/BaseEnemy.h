//#pragma once
//#include"Audio.h"
//#include"ObjParticle.h"
//#include"DirectXCommon.h"
//#include"Sprite.h"
//#include"ParticleManager.h"
//#include"Collision.h"
//
//using namespace DirectX;
//
//class BaseEnemy
//{
//private:
//	using XMFLOAT2 = DirectX::XMFLOAT2;
//	using XMFLOAT3 = DirectX::XMFLOAT3;
//	using XMFLOAT4 = DirectX::XMFLOAT4;
//	using XMVECTOR = DirectX::XMVECTOR;
//	using XMMATRIX = DirectX::XMMATRIX;
//public:
//	BaseEnemy();
//
//	virtual void Initialize(const XMFLOAT3& allrot, const XMVECTOR& allpos, Camera* camera, const XMVECTOR& trackpoint) = 0;
//	virtual void Update() = 0;
//	virtual void Draw() = 0;
//public:
//	void Damage();
//
//	void Death();
//
//	bool CheckHit(bool& playershot, XMFLOAT2 playerppos, XMFLOAT2 rockonpos, float distance, float mulvalue);
//
//	void ParticleEfect();
//
//	void Tracsparentsize();
//
//	void HitColor();
//
//private:
//
//	bool CheckShot(bool playershot);
//private:
//	std::list<std::unique_ptr<ObjParticle>>obj_particle_;
//	//ダメージを食らったときのエフェクト
//	ParticleManager* partgreen_ = nullptr;
//	ParticleManager* partred_ = nullptr;
//
//	XMVECTOR base_pos_ = {};
//	XMFLOAT3 base_rot_ = {};
//	float base_hp_ = {};
//	float old_hp_ = {};
//};
//
