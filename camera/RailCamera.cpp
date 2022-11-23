#include "RailCamera.h"

using namespace DirectX;


RailCamera::~RailCamera()
{
}

void RailCamera::Initialize(const XMVECTOR& Pos, const XMFLOAT3& Rot)
{
	//target = { 0.0f,0.0f,-1.0f };
	eye = { 0,1,-5 };

	//debugModel = ObjModel::CreateFromOBJ("Gear");
	//debug = Object3d::Create(debugModel);


	position = Pos;
	rotation = Rot;
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationX(XMConvertToRadians(rotation.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.y));
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));
	matTrans = XMMatrixTranslation(position.m128_f32[0], position.m128_f32[1], position.m128_f32[2]);
	matWorld = XMMatrixIdentity();
	matWorld *= matRot;
	matWorld *= matTrans;

	//ビュープロジェクションの初期化
	matViewProjection = XMMatrixIdentity();
}



void RailCamera::Updata(const XMVECTOR& vel, const XMFLOAT3& rot, Camera* NormalCam)
{
	position += vel;


	eye = { 0,1.5,-5 };
	rotation = { 0.0f,0.0f,0.0f };
	rotation.x = rot.x+rotation.x;
	rotation.y = rot.y+rotation.y;
	rotation.z = rot.z+rotation.z;

	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationX(XMConvertToRadians(rotation.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.y));
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));
	matTrans = XMMatrixIdentity();
	matTrans = XMMatrixTranslation(position.m128_f32[0], position.m128_f32[1], position.m128_f32[2]);
	matWorld = XMMatrixIdentity();
	matWorld *= matRot;
	matWorld *= matTrans;

	

	//matViewProjection = matView * matProjection;

	XMVECTOR eye2 = { eye.x,eye.y,eye.z };
	eye2 = XMVector3Transform(eye2, matWorld);
	//ワールド前方ベクトル
	XMVECTOR forward = { 0, 0, 1 };
	//レールカメラの回転を反映
	forward = XMVector3TransformNormal(forward, matWorld);

	//視点から前方に適当な距離進んだ位置が注視点
	target.x = eye2.m128_f32[0] + forward.m128_f32[0];
	target.y = eye2.m128_f32[1] + forward.m128_f32[1];
	target.z = eye2.m128_f32[2] + forward.m128_f32[2];

	//ワールド上方ベクトル
	XMVECTOR up = { 0,1,0 };
	//レールカメラの回転を反映(レールカメラの上方ベクトル)
	up = XMVector3TransformNormal(up, matWorld);
	XMFLOAT3 upp = { up.m128_f32[0],up.m128_f32[1],up.m128_f32[2] };
	eye = { eye2.m128_f32[0],eye2.m128_f32[1],eye2.m128_f32[2]};


	//ビュープロジェクションを更新
	NormalCam->SetTarget(target);
	NormalCam->SetUp(upp);
	NormalCam->SetEye(eye);
	//プレイヤーにワールド座標を送る
	NormalCam->SetWorld(matWorld);
	//debug->SetPosition(position);
	//debug->Updata();
}

void RailCamera::Draw()
{
	//debug->Draw();
}

void RailCamera::UpdataViewMatrix()
{
	// 視点座標
	eyePosition = XMLoadFloat3(&eye);
	// 注視点座標
	targetPosition = XMLoadFloat3(&target);
	// （仮の）上方向
	upVector = XMLoadFloat3(&up);

	// カメラZ軸（視線方向）
	XMVECTOR cameraAxisZ = XMVectorSubtract(targetPosition, eyePosition);
	// 0ベクトルだと向きが定まらないので除外
	assert(!XMVector3Equal(cameraAxisZ, XMVectorZero()));
	assert(!XMVector3IsInfinite(cameraAxisZ));
	assert(!XMVector3Equal(upVector, XMVectorZero()));
	assert(!XMVector3IsInfinite(upVector));
	// ベクトルを正規化
	cameraAxisZ = XMVector3Normalize(cameraAxisZ);

	// カメラのX軸（右方向）
	XMVECTOR cameraAxisX;
	// X軸は上方向→Z軸の外積で求まる
	cameraAxisX = XMVector3Cross(upVector, cameraAxisZ);
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
	matView = XMMatrixTranspose(matCameraRot);

	// 視点座標に-1を掛けた座標
	XMVECTOR reverseEyePosition = XMVectorNegate(eyePosition);
	// カメラの位置からワールド原点へのベクトル（カメラ座標系）
	XMVECTOR tX = XMVector3Dot(cameraAxisX, reverseEyePosition);	// X成分
	XMVECTOR tY = XMVector3Dot(cameraAxisY, reverseEyePosition);	// Y成分
	XMVECTOR tZ = XMVector3Dot(cameraAxisZ, reverseEyePosition);	// Z成分
	// 一つのベクトルにまとめる
	XMVECTOR translation = XMVectorSet(tX.m128_f32[0], tY.m128_f32[1], tZ.m128_f32[2], 1.0f);
	// ビュー行列に平行移動成分を設定
	matView.r[3] = translation;

}

void RailCamera::UpdataProjectionMatrix()
{
	// 透視投影による射影行列の生成
	matProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(60.0f),
		aspectRatio,
		0.1f, 1000.0f
	);
}


