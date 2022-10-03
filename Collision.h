#pragma once

#include"CollisionPrimitive.h"
#include<Windows.h>
#include<wrl.h>
#include<d3d12.h>
#include<d3dx12.h>
#include<DirectXMath.h>

class Collision
{
private: // �G�C���A�X
// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;

public:

	///<summary>
	///���ƕ��ʂ̓����蔻��
	///</summary>
	///<param name="sphere">��</param>
	///<param name="plane">����</param>
	///<param name="inter">��_(���ʏ�̍Őڋߓ_)</param>
	///<returns>�������Ă��邩�ۂ�</returns>
	static bool CheckSphere2Plane(const Sphere& sphere, const Plane& plane, DirectX::XMVECTOR* inter = nullptr);
	///<summary>
	///���ƎO�p�`�̍ŋߐړ_�����߂�
	///</summary>
	///<param name="point">�_</param>
	///<param name="triangle">�O�p�`</param>
	///<param name="closesr">�ŋߐړ_(�o�͗p)</param>
	static void ClosestPtPoint2Triangle(const DirectX::XMVECTOR& point, const Triangle& triangle, DirectX::XMVECTOR* closest);
	///<summary>
	///���ƎO�p�`�̍ŋߐړ_�����߂�
	///</summary>
	///<param name="point">�_</param>
	///<param name="triangle">�O�p�`</param>
	///<param name="inter">��_(�O�p�`��̍ŋߐړ_)</param>
	/// ///<returns>�������Ă��邩�ۂ�</returns>
	static bool CheckSpere2Triangle(const Sphere& sphere, const Triangle& triangle, DirectX::XMVECTOR* inter = nullptr);
	///<summary>
	///���C�ƕ��ʂ̓����蔻��
	/// </summary>
	/// <param name="lay">���C</param>
	/// <param name="plane">����</param>
	/// <param name="inter">����(�o�͗p)</param>
	/// <param name="inter">��_(�o�͗p)</param>
	/// <return>�������Ă��邩�ۂ�</return>
	static bool CheckRay2Plane(const Ray& ray, const Plane& plane, float* distance = nullptr, DirectX::XMVECTOR* inter = nullptr);
	///<summary>
	///���C�Ɩ@���t���̎O�p�`�̓����蔻��
	/// </summary>
	/// <param name="lay">���C</param>
	/// <param name="triangle">�O�p�`</param>
	/// <param name="inter">����(�o�͗p)</param>
	/// <param name="inter">��_(�o�͗p)</param>
	/// <return>�������Ă��邩�ۂ�</return>
	static bool CheckRay2Triangle(const Ray& ray, const Triangle& triangle, float* distance = nullptr, DirectX::XMVECTOR* inter = nullptr);

	void CheckPlayer2Enemy(XMFLOAT3 Playerposition, XMFLOAT3 Playerscale, XMFLOAT3 Enemyposition, XMFLOAT3 Enemyscale,int PlayerHp,int PlayerLevel);

	void CheckPlayer2Boss(XMFLOAT3 Playerposition, XMFLOAT3 Playerscale, XMFLOAT3 Bossposition, XMFLOAT3 Bossscale, int PlayerHp,int PlayerLevel);

	static bool Player2Other(XMVECTOR Playerposition, XMFLOAT3 Playerscale, XMVECTOR Otherposition, XMFLOAT3 Otherscale);

	static bool HeadShot(XMVECTOR Playerposition, XMFLOAT3 Playerscale, XMVECTOR Otherposition, XMFLOAT3 Otherscale);

	static bool BodyHit(XMVECTOR bodyPos, XMFLOAT3 bodyScl, XMVECTOR PlayerPos, XMFLOAT3 playerScl);

	static bool HeadHit(XMVECTOR headPos, XMFLOAT3 headScl, XMVECTOR PlayerPos, XMFLOAT3 playerScl);

	static bool ArmHit(XMVECTOR armPos, XMFLOAT3 armScl, XMVECTOR PlayerPos, XMFLOAT3 PlayerScl);
};

