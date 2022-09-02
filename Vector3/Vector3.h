#pragma once
#include<DirectXMath.h>

class Vector3
{
	struct VETOR {
		float x;
		float y;
		float z;
	};

public:
	Vector3();
	Vector3(float x, float y, float z);

	float length()const;//�m����(���������߂�)
	Vector3& normalize();//���K��
	float dot(const Vector3& v)const;//���ς����߂�
	Vector3 cross(const Vector3& v)const;//�O�ς����߂�

	Vector3 operator+() const;
	Vector3 operator-() const;

	Vector3& operator+=(const Vector3& v);
	Vector3& operator-=(const Vector3& v);
	Vector3& operator*=(float s);
	Vector3& operator/=(float s);

	float x;
	float y;
	float z;
};

const Vector3 operator+(const Vector3& v1, const Vector3& v2);
const Vector3 operator-(const Vector3& v1, const Vector3& v2);
const Vector3 operator*(const Vector3& v, float s);
const Vector3 operator*(float s, const Vector3& v);
const Vector3 operator/(const Vector3& v, float s);
