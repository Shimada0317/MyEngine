#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <string>
#include"camera/Camera.h"
class RailCamera
{
private: // �G�C���A�X
// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	RailCamera(int window_width, int window_height);

	~RailCamera();

	void Initialize();

	void Update();

	void UpdateViewMatrix();

	void UpdateProjectionMatrix();

private:
	Camera* camera = nullptr;

	XMMATRIX world;

	// ���[�J�����[���h�ϊ��s��
	XMMATRIX matWorld;

	XMMATRIX matRot, matTrans;

	XMFLOAT3 target = { 0,0,0 };
	XMFLOAT3 eye = { 0,0,0 };

	// �r���[�s��
	XMMATRIX matView = DirectX::XMMatrixIdentity();
	// �ˉe�s��
	XMMATRIX matProjection = DirectX::XMMatrixIdentity();
	// �r���[�ˉe�s��
	XMMATRIX matViewProjection = DirectX::XMMatrixIdentity();
	// ������x�N�g��
	XMFLOAT3 up = { 0, 1, 0 };
	// �A�X�y�N�g��
	float aspectRatio = 1.0f;
};

