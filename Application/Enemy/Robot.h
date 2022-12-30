#pragma once
#include"Body.h"
#include"RightArm.h"
#include"LeftArm.h"
#include"BothArms.h"
#include"Head.h"
#include"ObjParticle.h"
#include"DirectXCommon.h"
#include"Texture.h"
#include"TextureModel.h"
#include"Sprite.h"
#include"Camera.h"
#include<DirectXMath.h>
#include<memory>
#include<list>


class Robot
{
private:
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
public:
	//デストラクタ
	~Robot();
	//初期化
	void Initialize(const XMFLOAT3& allRot, const XMVECTOR& AllPos,const bool& Step=false);
	//全て更新
	void AllUpdata(Bullet* bull);
	//更新内の処理
	void Updata(Bullet* bull, const XMMATRIX& player,bool& spown,int& playerHp);
	//描画
	void Draw(DirectXCommon* dxCommon);
	//Img描画
	void ImgDraw();
	//パーティクル描画
	void ParticleDraw(DirectXCommon* dxCommon);
	//プレイヤー追尾
	void TrackPlayer();
	//首振り
	void Motion();
	//2D→3D座標
	void WorldtoScreen();
	//ビュー変換
	void ChangeViewPort(XMMATRIX& matViewPort);

public://Getter Setter
	void SetPosition(XMVECTOR allPos) { this->allPos = allPos; }

	void SetRotation(XMFLOAT3 allRot) { this->allRot = allRot; }

	void SetTrackPoint(XMVECTOR TrackPoint) { this->TrackPoint = TrackPoint; }

	const XMVECTOR& GetPosition() { return allPos; }

	bool IsDead() const { return isDead_; }

private:
	std::unique_ptr<Head> head;
	/*std::unique_ptr<RightArm> RArm;
	std::unique_ptr<LeftArm>LArm;*/
	std::unique_ptr<BothArms>Arms;
	std::unique_ptr<Body>body;
	std::unique_ptr<ObjParticle>part;
	std::unique_ptr<Object3d> shadow;
	ObjModel* shadowModel = nullptr;
	std::list<std::unique_ptr<ObjParticle>>particle_;

	std::unique_ptr<Object3d> center;

	std::unique_ptr<Sprite> RockOn;

	Texture* fireTex;

	XMFLOAT3 Texpos = { 0.0f,0.0f,0.0f };
	XMFLOAT3 TexSiz = { 1.0f,1.0f,1.0f };
	XMFLOAT4 TexCol = { 1.0f,1.0f,1.0f,1.0f };

	XMVECTOR alll = { 0.0f,0.0f,0.0f };

	XMVECTOR allPos = { 0.0f,0.0f,-10.0f };
	XMVECTOR firstPos = { 0.0f,0.0f,0.0f };
	XMFLOAT3 allRot = { 0.0f,0.0f,0.0f };
	//RockOn
	XMFLOAT2 RockOnPos = { 0.0f,0.0f };
	XMFLOAT2 RockOnAnc = { 0.5f,0.5f };
	XMFLOAT4 RockOnCol = { 1.0f,1.0f,1.0f,1.0f };

	XMVECTOR offset;

	XMMATRIX vmat;
	XMMATRIX cmat;
	XMMATRIX matViewPort;

	XMVECTOR imgpos={0.0f,0.0f,0.0f};

	Camera* camera;

	bool Partarive[3];
	bool Myarive = false;
	float attackT = 0.0f;
	float time = 0.0f;
	int Hp = 50;
	int OldHp = 0;

	XMFLOAT4 color = { 1,0,0,0.1f };

	XMVECTOR TrackPoint = { 0,0,0 };

	XMVECTOR playerPos;

	XMVECTOR shadowPos = { 0,0,0 };

	XMFLOAT4 shadowColor = { 0.0f,0.0f,0.0f,0.1f };

	int action = 0;
	float AttackChanse = 0;
	float rad = 0;
	bool AttackFase = false;
	bool shake = false;
	float AttackTime = 0;
	float speed = 0.005f;

	bool isDead_ = false;

	bool dice = false;
	int patern=0;


	float l = 0;

	float MotionTime = 0.0f;
	bool MotionChange = true;
	float RobT = 0.0f;
	bool slideF = false;
	float RobS = 0.001f;
	bool step = false;
};

