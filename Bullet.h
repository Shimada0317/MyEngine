#pragma once
#include<DirectXMath.h>
#include"Object3d.h"
#include"ObjModel.h"
#include"Action.h"
#include"Collision.h"

#include<memory>

class Player;
class Bullet
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	void Initialize();

	void bun(XMFLOAT3& pos, XMFLOAT3 position, float& speed, bool& shot, int& Remaining,bool& retime);

	void Shot(XMFLOAT3& pos, XMFLOAT2 position, int speed, bool& shot);

	void debug(XMFLOAT3& pos, XMFLOAT3 posiiton, int speed,bool& d);

	void Set();

	void Update();

	void Draw();

	void ImgUiDraw();

	void Finalize();

	const XMFLOAT3& GetPosition() { return pos; }

	void SetPosition(XMFLOAT3 position) { this->pos = position; }

	const XMFLOAT3& GetRotation() { return rot; }

	void SetRotation(XMFLOAT3 rotation) { this->rot = rotation; }

	const XMFLOAT3& GetScl() { return scl; }

	void SetScl(XMFLOAT3 scale) { this->scl = scale; }

	const bool& GetLost() { return lost; }

	void SetLost(bool lost) { this->lost = lost; }
private:
	std::unique_ptr<Object3d> bullet;
	ObjModel* bulModel = nullptr;


	XMFLOAT3 pos = { 0.0f,0.0f,-100.0f };
	XMFLOAT3 oldpos{ 0.0f,0.0f,0.0f };
	XMFLOAT3 scl = { 0.1f,0.1f,0.1f };
	XMFLOAT3 rot = { 0.0f,0.0f,0.0f };

	bool shot = false;
	int rebun = 0;
	float speed = 1.0f;
	float Loadtime = 0.0f;

	bool lost = false;



	//debug
	bool d = false;
};