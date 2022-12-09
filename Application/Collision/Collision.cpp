#include "Collision.h"
#include<math.h>

using namespace DirectX;

bool Collision::CheckSphere2Plane(const Sphere& sphere, const Plane& plane, DirectX::XMVECTOR* inter)
{
	//���W�n�̌��_���狅�̒��S���W�ւ̋���
	XMVECTOR distV = XMVector3Dot(sphere.center, plane.normal);
	//���ʂ̌��_���������Z���邱�ƂŁA���ʂƋ��̒��S�Ƃ̋������o��
	float dist = distV.m128_f32[0] - plane.distance;
	//�����̐�Βl�����a���傫����Γ������Ă��Ȃ�
	if (fabsf(dist) > sphere.radius) return false;
	//�^����_���v�Z
	if (inter) {
		*inter = -dist * plane.normal + sphere.center;
	}

	return true;
}

void Collision::ClosestPtPoint2Triangle(const DirectX::XMVECTOR& point, const Triangle& triangle, DirectX::XMVECTOR* closest)
{
	// point��p0�̊O���̒��_�̈�̒��ɂ��邩�ǂ����`�F�b�N
	XMVECTOR p0_p1 = triangle.p1 - triangle.p0;
	XMVECTOR p0_p2 = triangle.p2 - triangle.p0;
	XMVECTOR p0_pt = point - triangle.p0;

	XMVECTOR d1 = XMVector3Dot(p0_p1, p0_pt);
	XMVECTOR d2 = XMVector3Dot(p0_p2, p0_pt);

	if (d1.m128_f32[0] <= 0.0f && d2.m128_f32[0] <= 0.0f)
	{
		// p0���ŋߖT
		*closest = triangle.p0;
		return;
	}

	// point��p1�̊O���̒��_�̈�̒��ɂ��邩�ǂ����`�F�b�N
	XMVECTOR p1_pt = point - triangle.p1;

	XMVECTOR d3 = XMVector3Dot(p0_p1, p1_pt);
	XMVECTOR d4 = XMVector3Dot(p0_p2, p1_pt);

	if (d3.m128_f32[0] >= 0.0f && d4.m128_f32[0] <= d3.m128_f32[0])
	{
		// p1���ŋߖT
		*closest = triangle.p1;
		return;
	}

	// point��p0_p1�̕ӗ̈�̒��ɂ��邩�ǂ����`�F�b�N���A�����point��p0_p1��ɑ΂���ˉe��Ԃ�
	float vc = d1.m128_f32[0] * d4.m128_f32[0] - d3.m128_f32[0] * d2.m128_f32[0];
	if (vc <= 0.0f && d1.m128_f32[0] >= 0.0f && d3.m128_f32[0] <= 0.0f)
	{
		float v = d1.m128_f32[0] / (d1.m128_f32[0] - d3.m128_f32[0]);
		*closest = triangle.p0 + v * p0_p1;
		return;
	}

	// point��p2�̊O���̒��_�̈�̒��ɂ��邩�ǂ����`�F�b�N
	XMVECTOR p2_pt = point - triangle.p2;

	XMVECTOR d5 = XMVector3Dot(p0_p1, p2_pt);
	XMVECTOR d6 = XMVector3Dot(p0_p2, p2_pt);
	if (d6.m128_f32[0] >= 0.0f && d5.m128_f32[0] <= d6.m128_f32[0])
	{
		*closest = triangle.p2;
		return;
	}

	// point��p0_p2�̕ӗ̈�̒��ɂ��邩�ǂ����`�F�b�N���A�����point��p0_p2��ɑ΂���ˉe��Ԃ�
	float vb = d5.m128_f32[0] * d2.m128_f32[0] - d1.m128_f32[0] * d6.m128_f32[0];
	if (vb <= 0.0f && d2.m128_f32[0] >= 0.0f && d6.m128_f32[0] <= 0.0f)
	{
		float w = d2.m128_f32[0] / (d2.m128_f32[0] - d6.m128_f32[0]);
		*closest = triangle.p0 + w * p0_p2;
		return;
	}

	// point��p1_p2�̕ӗ̈�̒��ɂ��邩�ǂ����`�F�b�N���A�����point��p1_p2��ɑ΂���ˉe��Ԃ�
	float va = d3.m128_f32[0] * d6.m128_f32[0] - d5.m128_f32[0] * d4.m128_f32[0];
	if (va <= 0.0f && (d4.m128_f32[0] - d3.m128_f32[0]) >= 0.0f && (d5.m128_f32[0] - d6.m128_f32[0]) >= 0.0f)
	{
		float w = (d4.m128_f32[0] - d3.m128_f32[0]) / ((d4.m128_f32[0] - d3.m128_f32[0]) + (d5.m128_f32[0] - d6.m128_f32[0]));
		*closest = triangle.p1 + w * (triangle.p2 - triangle.p1);
		return;
	}

	float denom = 1.0f / (va + vb + vc);
	float v = vb * denom;
	float w = vc * denom;
	*closest = triangle.p0 + p0_p1 * v + p0_p2 * w;
}

bool Collision::CheckSpere2Triangle(const Sphere& sphere, const Triangle& triangle, DirectX::XMVECTOR* inter)
{
	XMVECTOR p;
	//���̒��S�ɑ΂���ŋߐړ_�ł���O�p�`��ɂ���_p��������
	ClosestPtPoint2Triangle(sphere.center, triangle, &p);
	//�_p�Ƌ��̒��S�̍����x�N�g��
	XMVECTOR v = p - sphere.center;
	//�����̓������߂�
	//(�����x�N�g�����m�̓��ς͎O�����̒藝�̃��[�g�����̎��ƈ�v����)
	v = XMVector3Dot(v, v);
	//���ƎO�p�`�̋��������a�ȉ��Ȃ瓖�����Ă��Ȃ�
	if (v.m128_f32[0] > sphere.radius * sphere.radius) return false;

	if (inter) {
		*inter = p;
	}
	return true;
}

bool Collision::CheckRay2Plane(const Ray& ray, const Plane& plane, float* distance, DirectX::XMVECTOR* inter)
{
	const float epslion = 1.0e-5f;//�덷�z���p�̔����Ȓl
	//�ʖ@���ƃ��C�̕����x�N�g������
	float d1 = XMVector3Dot(plane.normal, ray.dir).m128_f32[0];
	//���ʂɂ͓�����Ȃ�
	if (d1 > -epslion) { return false; }
	//�n�_�ƌ��_�̋���(���ʂ̖@������)
	//�ʖ@���ƃ��C�̎n�_���W(�ʒu�x�N�g��)�̓���
	float d2 = XMVector3Dot(plane.normal, ray.start).m128_f32[0];
	//�n�_�ƕ��ʂ̋���
	float dist = d2 - plane.distance;
	//�n�_�ƕ��ʂ̋���(���C����)
	float t = dist / -d1;
	//��_���n�_�����ɂ���̂ŁA������Ȃ�
	if (t < 0)return false;
	//��������������
	if (distance) { *distance = t; }
	//��_���v�Z
	if (inter) { *inter = ray.start + t * ray.dir; }
	return true;
}

bool Collision::CheckRay2Triangle(const Ray& ray, const Triangle& trieangle, float* distance, DirectX::XMVECTOR* inter)
{
	//�O�p�`������Ă��镽�ʂ��Z�o
	Plane plane;
	XMVECTOR interPlane;

	plane.normal = trieangle.normal;
	plane.distance = XMVector3Dot(trieangle.normal, trieangle.p0).m128_f32[0];
	//���C�ƕ��ʂ��������Ă��Ȃ���΁A�������Ă��Ȃ�
	if (!CheckRay2Plane(ray, plane, distance, &interPlane)) { return false; }
	//���C�ƕ��ʂ��������Ă����̂ŁA�����ƌ�_���������܂ꂽ
	//���C�ƕ��ʂ̌�_���O�p�`�̓����ɂ��邩����
	const float epsilon = 1.0e-5f;//�덷�z���p�̔����Ȓl
	XMVECTOR m;
	//��p0_p1�ɂ���
	XMVECTOR pt_p0 = trieangle.p0 - interPlane;
	XMVECTOR p0_p1 = trieangle.p1 - trieangle.p0;
	m = XMVector3Cross(pt_p0, p0_p1);
	//�ӂ̓������������Ă��Ȃ��̂Ŕ����ł��؂�
	if (XMVector3Dot(m, trieangle.normal).m128_f32[0] < -epsilon) { return false; }
	//��p1_p2�ɂ���
	XMVECTOR pt_p1 = trieangle.p1 - interPlane;
	XMVECTOR p1_p2 = trieangle.p1 - trieangle.p2;
	m = XMVector3Cross(pt_p1, p1_p2);
	//�ӂ̊O���ł���Γ������Ă��Ȃ��̂Ŕ����ł��؂�
	if (XMVector3Dot(m, trieangle.normal).m128_f32[0] < -epsilon) { return false; }
	//��p2_p0�ɂ���
	XMVECTOR pt_p2 = trieangle.p2 - interPlane;
	XMVECTOR p2_p0 = trieangle.p2 - trieangle.p0;
	m = XMVector3Cross(pt_p2, p2_p0);
	//�ӂ̊O���ł���Γ������Ă��Ȃ��̂Ŕ����ł��؂�
	if (XMVector3Dot(m, trieangle.normal).m128_f32[0] < -epsilon) { return false; }
	//�����Ȃ̂œ������Ă���
	if (inter) {
		*inter = interPlane;
	}
	return true;
}


void Collision::CheckPlayer2Enemy(XMFLOAT3 Playerposition, XMFLOAT3 Playerscale, XMFLOAT3 Enemyposition, XMFLOAT3 Enemyscale, int PlayerHp, int PlayerLevel)
{
	/*const int attack = 5;
	if (Player2Other(Playerposition, Playerscale, Enemyposition, Enemyscale) == true) {
		PlayerHp = PlayerHp - attack;
	}*/
}

void Collision::CheckPlayer2Boss(XMFLOAT3 Playerposition, XMFLOAT3 Playerscale, XMFLOAT3 Bossposition, XMFLOAT3 Bossscale, int PlayerHp, int PlayerLevel)
{
	//const int attack = 10;
	//if (Player2Other(Playerposition, Playerscale, Bossposition, Bossscale) == true) {
	//	PlayerHp = PlayerHp - attack;
	//}

}

bool Collision::Player2Other(XMVECTOR Playerposition, XMFLOAT3 Playerscale, XMVECTOR Otherposition, XMFLOAT3 Otherscale)
{
	float x = Playerposition.m128_f32[0] - Otherposition.m128_f32[0];
	float y = Playerposition.m128_f32[1] - Otherposition.m128_f32[1];
	float z = Playerposition.m128_f32[2] - Otherposition.m128_f32[2];

	float PlayerAllscale = Playerscale.x + Playerscale.y + Playerscale.z;
	float EnemyAllscale = (Otherscale.x + Otherscale.y + Otherscale.z) / 4;

	float length = sqrt(x * x + y * y + z * z);

	if (length <= PlayerAllscale + EnemyAllscale) {
		return true;
	}
	else {
		return false;
	}
}

bool Collision::HeadShot(XMVECTOR Playerposition, XMFLOAT3 Playerscale, XMVECTOR Otherposition, XMFLOAT3 Otherscale)
{
	Otherposition.m128_f32[1] = Otherposition.m128_f32[1] + 2;

	float x = Playerposition.m128_f32[0] - Otherposition.m128_f32[0];
	float y = Playerposition.m128_f32[1] - Otherposition.m128_f32[1];
	float z = Playerposition.m128_f32[2] - Otherposition.m128_f32[2];

	float PlayerAllscale = Playerscale.x + Playerscale.y + Playerscale.z;
	float EnemyAllscale = (Otherscale.x + Otherscale.y + Otherscale.z) / 4;

	float length = sqrt(x * x + y * y + z * z);

	if (length <= PlayerAllscale + EnemyAllscale) {
		return true;
	}
	else {
		return false;
	}
}

bool Collision::BodyHit(XMVECTOR bodyPos, XMFLOAT3 bodyScl, XMVECTOR PlayerPos, XMFLOAT3 playerScl)
{

	float mbx = bodyPos.m128_f32[0] - bodyScl.x;
	float pbx = bodyPos.m128_f32[0] + bodyScl.x;
	float mby = bodyPos.m128_f32[1] - bodyScl.y/2;
	float pby = bodyPos.m128_f32[1] + bodyScl.y/2;
	float mbz = bodyPos.m128_f32[2] - bodyScl.z;
	float pbz = bodyPos.m128_f32[2] + bodyScl.z;

	float mpx = PlayerPos.m128_f32[0] - playerScl.x / 2;
	float ppx = PlayerPos.m128_f32[0] + playerScl.x / 2;
	float mpy = PlayerPos.m128_f32[1] - playerScl.y / 2;
	float ppy = PlayerPos.m128_f32[1] + playerScl.y / 2;
	float mpz = PlayerPos.m128_f32[2] - playerScl.z / 2;
	float ppz = PlayerPos.m128_f32[2] + playerScl.z / 2;

	/*if ((mbx <= ppx && mpx >= pbx || mby <= ppy && mby >= ppy) && mbz <= ppz) {
		return true;
	}*/

	if ((mbx <= ppx && mpx <= pbx) && (mby <= ppy && mpy <= pby) && (mbz <= ppz && mpz <= pbz)) {
		return true;
	}


	return false;
}

bool Collision::HeadHit(XMVECTOR headPos, XMFLOAT3 headScl, XMVECTOR PlayerPos, XMFLOAT3 playerScl)
{
	float mbx = headPos.m128_f32[0] - headScl.x*1.5f;
	float pbx = headPos.m128_f32[0] + headScl.x*1.5f;
	float mby = headPos.m128_f32[1] - headScl.y;
	float pby = headPos.m128_f32[1] + headScl.y*1.2f;
	float mbz = headPos.m128_f32[2] - headScl.z * 1.2f;
	float pbz = headPos.m128_f32[2] + headScl.z * 1.2f;

	float mpx = PlayerPos.m128_f32[0] - playerScl.x / 2;
	float ppx = PlayerPos.m128_f32[0] + playerScl.x / 2;
	float mpy = PlayerPos.m128_f32[1] - playerScl.y / 2;
	float ppy = PlayerPos.m128_f32[1] + playerScl.y / 2;
	float mpz = PlayerPos.m128_f32[2] - playerScl.z / 2;
	float ppz = PlayerPos.m128_f32[2] + playerScl.z / 2;

	if ((mbx <= ppx && mpx <= pbx && mby <= ppy && mpy <= pby) && (mbz <= ppz && mpz <= pbz)) {
		return true;
	}

	return false;
}

bool Collision::ArmHit(XMVECTOR armPos, XMFLOAT3 armScl, XMVECTOR PlayerPos, XMFLOAT3 playerScl)
{
	float max = armPos.m128_f32[0] - armScl.x / 4;
	float pax = armPos.m128_f32[0] + armScl.x / 4;
	float may = armPos.m128_f32[1] - armScl.y;
	float pay = armPos.m128_f32[1] + armScl.y;
	float maz = armPos.m128_f32[2] - armScl.z;
	float paz = armPos.m128_f32[2] + armScl.z;

	float mpx = PlayerPos.m128_f32[0] - playerScl.x / 4;
	float ppx = PlayerPos.m128_f32[0] + playerScl.x / 4;
	float mpy = PlayerPos.m128_f32[1] - playerScl.y / 4;
	float ppy = PlayerPos.m128_f32[1] + playerScl.y / 4;
	float mpz = PlayerPos.m128_f32[2] - playerScl.z;
	float ppz = PlayerPos.m128_f32[2] + playerScl.z;

	if ((max <= ppx && mpx <= pax && may <= ppy && mpy <= pay) && (maz <= ppz && mpz <= paz)) {
		return true;
	}

	return false;
}
