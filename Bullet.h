#pragma once
#include<DirectXMath.h>
#include"Object3d.h"
#include"ObjModel.h"
#include"Action.h"
#include"Collision.h"
#include <d3dx12.h>

#include<memory>

using namespace DirectX;
using namespace Microsoft::WRL;
using namespace std;

class Player;
class Bullet
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::‚ðÈ—ª
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;
public:
	void Initialize();

	void Stanby(const XMVECTOR& playerPos,const XMVECTOR& vel);

	void bun(XMVECTOR& pos, XMVECTOR position, float& speed, bool& shot, int& Remaining,bool& retime);

	void Shot(XMVECTOR& pos, XMFLOAT2 position, int speed, bool& shot);

	void debug(XMFLOAT3& pos, XMFLOAT3 posiiton, int speed,bool& d);

	void Set();

	void Update();

	void Draw();

	void ImgUiDraw();

	void Finalize();

	const XMVECTOR& GetPosition() { return pos; }

	void SetPosition(XMVECTOR position) { this->pos = position; }

	const XMFLOAT3& GetRotation() { return rot; }

	void SetRotation(XMFLOAT3 rotation) { this->rot = rotation; }

	const XMFLOAT3& GetScl() { return scl; }

	void SetScl(XMFLOAT3 scale) { this->scl = scale; }

	const bool& GetLost() { return lost; }

	void SetLost(bool lost) { this->lost = lost; }
private:
	std::unique_ptr<Object3d> bullet;
	ObjModel* bulModel = nullptr;


	XMVECTOR pos = { 0.0f,0.0f,-100.0f };
	XMVECTOR oldpos{ 0.0f,0.0f,0.0f };
	XMFLOAT3 scl = { 0.1f,0.1f,0.1f };
	XMFLOAT3 rot = { 0.0f,-90.0f,0.0f };

	bool shot = false;
	int rebun = 0;
	float speed = 1.0f;
	float Loadtime = 0.0f;

	bool lost = false;

	XMVECTOR velocity_;
	XMMATRIX mat;
	XMMATRIX matvel;
	//debug
	bool d = false;
};