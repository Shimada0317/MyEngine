#pragma once
#include<DirectXMath.h>
#include"Object3d.h"
#include"ObjModel.h"
#include"Action.h"
#include"Collision.h"

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
public:
	void Initialize();

	void bun(XMFLOAT3 pos);

	void Update();

	void Draw();

	void ImgUiDraw();

	void Finalize();

	void SetPos();

	const XMFLOAT3& GetPosition() { return pos; }

	void SetPosition(XMFLOAT3 position) { this->pos = position; }

	const XMFLOAT3& GetScale() { return scl; }

	void SetScale(XMFLOAT3 scale) { this->scl = scale; }
private:
	Object3d* bullet = nullptr;
	ObjModel* bulModel = nullptr;


	XMFLOAT3 pos = { 1.0f,1.0f,-10.0f };
	XMFLOAT3 oldpos{ 0.0f,0.0f,0.0f };
	XMFLOAT3 scl = { 1.0f,1.0f,0.01f };

	bool shot = false;
	float speed = 1.0f;
};

