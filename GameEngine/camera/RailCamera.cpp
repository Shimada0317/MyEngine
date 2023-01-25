#include "RailCamera.h"

using namespace DirectX;


RailCamera::~RailCamera()
{
}

void RailCamera::MatrixIdentity(const XMVECTOR& Pos, const XMFLOAT3& Rot)
{
	Eye = { 0,1,-5 };

	Position = Pos;
	Rotation = Rot;
	MatRot = XMMatrixIdentity();
	MatRot *= XMMatrixRotationX(XMConvertToRadians(Rotation.x));
	MatRot *= XMMatrixRotationY(XMConvertToRadians(Rotation.y));
	MatRot *= XMMatrixRotationZ(XMConvertToRadians(Rotation.z));
	MatTrans = XMMatrixTranslation(Position.m128_f32[0], Position.m128_f32[1], Position.m128_f32[2]);
	MatWorld = XMMatrixIdentity();
	MatWorld *= MatRot;
	MatWorld *= MatTrans;

	//ビュープロジェクションの初期化
	MatViewProjection = XMMatrixIdentity();
}



void RailCamera::Update(const XMVECTOR& vel, const XMFLOAT3& rot, Camera* NormalCam)
{
	Position += vel;

	Eye = { 0,1.5,-5 };
	Rotation = { 0.0f,0.0f,0.0f };
	Rotation.x = rot.x;
	Rotation.y = rot.y;
	Rotation.z = rot.z;

	MatRot = XMMatrixIdentity();
	MatRot *= XMMatrixRotationX(XMConvertToRadians(Rotation.x));
	MatRot *= XMMatrixRotationY(XMConvertToRadians(Rotation.y));
	MatRot *= XMMatrixRotationZ(XMConvertToRadians(Rotation.z));
	MatTrans = XMMatrixIdentity();
	MatTrans = XMMatrixTranslation(Position.m128_f32[0], Position.m128_f32[1], Position.m128_f32[2]);
	MatWorld = XMMatrixIdentity();
	MatWorld *= MatRot;
	MatWorld *= MatTrans;

	XMVECTOR eye2 = { Eye.x,Eye.y,Eye.z };
	eye2 = XMVector3Transform(eye2, MatWorld);
	//ワールド前方ベクトル
	XMVECTOR forward = { 0, 0, 1 };
	//レールカメラの回転を反映
	forward = XMVector3TransformNormal(forward, MatWorld);

	//視点から前方に適当な距離進んだ位置が注視点
	Target.x = eye2.m128_f32[0] + forward.m128_f32[0];
	Target.y = eye2.m128_f32[1] + forward.m128_f32[1];
	Target.z = eye2.m128_f32[2] + forward.m128_f32[2];

	//ワールド上方ベクトル
	XMVECTOR Up = { 0,1,0 };
	//レールカメラの回転を反映(レールカメラの上方ベクトル)
	Up = XMVector3TransformNormal(Up, MatWorld);
	XMFLOAT3 upp = { Up.m128_f32[0],Up.m128_f32[1],Up.m128_f32[2] };
	Eye = { eye2.m128_f32[0],eye2.m128_f32[1],eye2.m128_f32[2]};


	//ビュープロジェクションを更新
	NormalCam->SetTarget(Target);
	NormalCam->SetUp(upp);
	NormalCam->SetEye(Eye);
	//プレイヤーにワールド座標を送る
	NormalCam->SetWorld(MatWorld);

}

void RailCamera::ShakeCamera(const XMVECTOR& Pos)
{
	Position = Pos;

	Position.m128_f32[0] += 0.1f;
	Position.m128_f32[1] += 0.1f;
}
