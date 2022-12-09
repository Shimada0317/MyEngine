#include "Collision.h"
#include<math.h>

using namespace DirectX;

bool Collision::CheckSphere2Plane(const Sphere& sphere, const Plane& plane, DirectX::XMVECTOR* inter)
{
	//座標系の原点から球の中心座標への距離
	XMVECTOR distV = XMVector3Dot(sphere.center, plane.normal);
	//平面の原点距離を減算することで、平面と球の中心との距離が出る
	float dist = distV.m128_f32[0] - plane.distance;
	//距離の絶対値が半径より大きければ当たっていない
	if (fabsf(dist) > sphere.radius) return false;
	//疑似交点を計算
	if (inter) {
		*inter = -dist * plane.normal + sphere.center;
	}

	return true;
}

void Collision::ClosestPtPoint2Triangle(const DirectX::XMVECTOR& point, const Triangle& triangle, DirectX::XMVECTOR* closest)
{
	// pointがp0の外側の頂点領域の中にあるかどうかチェック
	XMVECTOR p0_p1 = triangle.p1 - triangle.p0;
	XMVECTOR p0_p2 = triangle.p2 - triangle.p0;
	XMVECTOR p0_pt = point - triangle.p0;

	XMVECTOR d1 = XMVector3Dot(p0_p1, p0_pt);
	XMVECTOR d2 = XMVector3Dot(p0_p2, p0_pt);

	if (d1.m128_f32[0] <= 0.0f && d2.m128_f32[0] <= 0.0f)
	{
		// p0が最近傍
		*closest = triangle.p0;
		return;
	}

	// pointがp1の外側の頂点領域の中にあるかどうかチェック
	XMVECTOR p1_pt = point - triangle.p1;

	XMVECTOR d3 = XMVector3Dot(p0_p1, p1_pt);
	XMVECTOR d4 = XMVector3Dot(p0_p2, p1_pt);

	if (d3.m128_f32[0] >= 0.0f && d4.m128_f32[0] <= d3.m128_f32[0])
	{
		// p1が最近傍
		*closest = triangle.p1;
		return;
	}

	// pointがp0_p1の辺領域の中にあるかどうかチェックし、あればpointのp0_p1上に対する射影を返す
	float vc = d1.m128_f32[0] * d4.m128_f32[0] - d3.m128_f32[0] * d2.m128_f32[0];
	if (vc <= 0.0f && d1.m128_f32[0] >= 0.0f && d3.m128_f32[0] <= 0.0f)
	{
		float v = d1.m128_f32[0] / (d1.m128_f32[0] - d3.m128_f32[0]);
		*closest = triangle.p0 + v * p0_p1;
		return;
	}

	// pointがp2の外側の頂点領域の中にあるかどうかチェック
	XMVECTOR p2_pt = point - triangle.p2;

	XMVECTOR d5 = XMVector3Dot(p0_p1, p2_pt);
	XMVECTOR d6 = XMVector3Dot(p0_p2, p2_pt);
	if (d6.m128_f32[0] >= 0.0f && d5.m128_f32[0] <= d6.m128_f32[0])
	{
		*closest = triangle.p2;
		return;
	}

	// pointがp0_p2の辺領域の中にあるかどうかチェックし、あればpointのp0_p2上に対する射影を返す
	float vb = d5.m128_f32[0] * d2.m128_f32[0] - d1.m128_f32[0] * d6.m128_f32[0];
	if (vb <= 0.0f && d2.m128_f32[0] >= 0.0f && d6.m128_f32[0] <= 0.0f)
	{
		float w = d2.m128_f32[0] / (d2.m128_f32[0] - d6.m128_f32[0]);
		*closest = triangle.p0 + w * p0_p2;
		return;
	}

	// pointがp1_p2の辺領域の中にあるかどうかチェックし、あればpointのp1_p2上に対する射影を返す
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
	//球の中心に対する最近接点である三角形上にある点pを見つける
	ClosestPtPoint2Triangle(sphere.center, triangle, &p);
	//点pと球の中心の差分ベクトル
	XMVECTOR v = p - sphere.center;
	//距離の二乗を求める
	//(同じベクトル同士の内積は三平方の定理のルート内部の式と一致する)
	v = XMVector3Dot(v, v);
	//球と三角形の距離が半径以下なら当たっていない
	if (v.m128_f32[0] > sphere.radius * sphere.radius) return false;

	if (inter) {
		*inter = p;
	}
	return true;
}

bool Collision::CheckRay2Plane(const Ray& ray, const Plane& plane, float* distance, DirectX::XMVECTOR* inter)
{
	const float epslion = 1.0e-5f;//誤差吸収用の微小な値
	//面法線とレイの方向ベクトル内積
	float d1 = XMVector3Dot(plane.normal, ray.dir).m128_f32[0];
	//裏面には当たらない
	if (d1 > -epslion) { return false; }
	//始点と原点の距離(平面の法線方向)
	//面法線とレイの始点座標(位置ベクトル)の内積
	float d2 = XMVector3Dot(plane.normal, ray.start).m128_f32[0];
	//始点と平面の距離
	float dist = d2 - plane.distance;
	//始点と平面の距離(レイ方向)
	float t = dist / -d1;
	//交点が始点より後ろにあるので、当たらない
	if (t < 0)return false;
	//距離を書き込む
	if (distance) { *distance = t; }
	//交点を計算
	if (inter) { *inter = ray.start + t * ray.dir; }
	return true;
}

bool Collision::CheckRay2Triangle(const Ray& ray, const Triangle& trieangle, float* distance, DirectX::XMVECTOR* inter)
{
	//三角形が乗っている平面を算出
	Plane plane;
	XMVECTOR interPlane;

	plane.normal = trieangle.normal;
	plane.distance = XMVector3Dot(trieangle.normal, trieangle.p0).m128_f32[0];
	//レイと平面が当たっていなければ、当たっていない
	if (!CheckRay2Plane(ray, plane, distance, &interPlane)) { return false; }
	//レイと平面が当たっていたので、距離と交点が書き込まれた
	//レイと平面の交点が三角形の内側にあるか判定
	const float epsilon = 1.0e-5f;//誤差吸収用の微小な値
	XMVECTOR m;
	//辺p0_p1について
	XMVECTOR pt_p0 = trieangle.p0 - interPlane;
	XMVECTOR p0_p1 = trieangle.p1 - trieangle.p0;
	m = XMVector3Cross(pt_p0, p0_p1);
	//辺の内側が当たっていないので判定を打ち切る
	if (XMVector3Dot(m, trieangle.normal).m128_f32[0] < -epsilon) { return false; }
	//辺p1_p2について
	XMVECTOR pt_p1 = trieangle.p1 - interPlane;
	XMVECTOR p1_p2 = trieangle.p1 - trieangle.p2;
	m = XMVector3Cross(pt_p1, p1_p2);
	//辺の外側であれば当たっていないので判定を打ち切る
	if (XMVector3Dot(m, trieangle.normal).m128_f32[0] < -epsilon) { return false; }
	//辺p2_p0について
	XMVECTOR pt_p2 = trieangle.p2 - interPlane;
	XMVECTOR p2_p0 = trieangle.p2 - trieangle.p0;
	m = XMVector3Cross(pt_p2, p2_p0);
	//辺の外側であれば当たっていないので判定を打ち切る
	if (XMVector3Dot(m, trieangle.normal).m128_f32[0] < -epsilon) { return false; }
	//内側なので当たっている
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
