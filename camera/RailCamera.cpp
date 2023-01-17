#include "RailCamera.h"

using namespace DirectX;


RailCamera::~RailCamera()
{
}

void RailCamera::Initialize(const XMVECTOR& Pos, const XMFLOAT3& Rot)
{
	//Target = { 0.0f,0.0f,-1.0f };
	Eye = { 0,1,-5 };

	//debugModel = ObjModel::CreateFromOBJ("Gear");
	//debug = Object3d::Create(debugModel);


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



void RailCamera::Updata(const XMVECTOR& vel, const XMFLOAT3& rot, Camera* NormalCam)
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
	//debug->SetPosition(Position);
	//debug->Updata();
}

void RailCamera::Draw()
{
	//debug->Draw();
}

void RailCamera::UpdataViewMatrix()
{
	// 視点座標
	EyePosition = XMLoadFloat3(&Eye);
	// 注視点座標
	TargetPosition = XMLoadFloat3(&Target);
	// （仮の）上方向
	UpVector = XMLoadFloat3(&Up);

	// カメラZ軸（視線方向）
	XMVECTOR cameraAxisZ = XMVectorSubtract(TargetPosition, EyePosition);
	// 0ベクトルだと向きが定まらないので除外
	assert(!XMVector3Equal(cameraAxisZ, XMVectorZero()));
	assert(!XMVector3IsInfinite(cameraAxisZ));
	assert(!XMVector3Equal(UpVector, XMVectorZero()));
	assert(!XMVector3IsInfinite(UpVector));
	// ベクトルを正規化
	cameraAxisZ = XMVector3Normalize(cameraAxisZ);

	// カメラのX軸（右方向）
	XMVECTOR cameraAxisX;
	// X軸は上方向→Z軸の外積で求まる
	cameraAxisX = XMVector3Cross(UpVector, cameraAxisZ);
	// ベクトルを正規化
	cameraAxisX = XMVector3Normalize(cameraAxisX);

	// カメラのY軸（上方向）
	XMVECTOR cameraAxisY;
	// Y軸はZ軸→X軸の外積で求まる
	cameraAxisY = XMVector3Cross(cameraAxisZ, cameraAxisX);

	// ここまでで直交した3方向のベクトルが揃う
	//（ワールド座標系でのカメラの右方向、上方向、前方向）	

	// カメラ回転行列
	XMMATRIX matCameraRot;
	// カメラ座標系→ワールド座標系の変換行列
	matCameraRot.r[0] = cameraAxisX;
	matCameraRot.r[1] = cameraAxisY;
	matCameraRot.r[2] = cameraAxisZ;
	matCameraRot.r[3] = XMVectorSet(0, 0, 0, 1);
	// 転置により逆行列（逆回転）を計算
	MatView = XMMatrixTranspose(matCameraRot);

	// 視点座標に-1を掛けた座標
	XMVECTOR reverseEyePosition = XMVectorNegate(EyePosition);
	// カメラの位置からワールド原点へのベクトル（カメラ座標系）
	XMVECTOR tX = XMVector3Dot(cameraAxisX, reverseEyePosition);	// X成分
	XMVECTOR tY = XMVector3Dot(cameraAxisY, reverseEyePosition);	// Y成分
	XMVECTOR tZ = XMVector3Dot(cameraAxisZ, reverseEyePosition);	// Z成分
	// 一つのベクトルにまとめる
	XMVECTOR translation = XMVectorSet(tX.m128_f32[0], tY.m128_f32[1], tZ.m128_f32[2], 1.0f);
	// ビュー行列に平行移動成分を設定
	MatView.r[3] = translation;

}

void RailCamera::UpdataProjectionMatrix()
{
	// 透視投影による射影行列の生成
	MatProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(60.0f),
		AspectRatio,
		0.1f, 1000.0f
	);
}


