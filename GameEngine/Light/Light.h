#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include<string>

#pragma once
class Light
{
private: // �G�C���A�X
// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
public://�T�u�N���X
	struct ConstBufferData {
		XMVECTOR lightv;//���C�g�̕����x�N�g��
		XMFLOAT3 lightcolor;//���C�g�̐F
	};
private://�ÓI�����o�ϐ�
	//�f�o�C�X
	static ID3D12Device* dev;
public://�ÓI�����o�֐�

	static void StaticInitialize(ID3D12Device* dev);

	static std::unique_ptr<Light>Create();
private://�����o�ϐ�
	ComPtr<ID3D12Resource> constBuff;
	//���C�g�̌�������
	XMVECTOR lightdir = { 1,0,0,0 };
	//���C�g�̐F
	XMFLOAT3 lightcolor = { 1,1,1 };
	//�_�[�e�B�t���O
	bool dirty = false;
public://�����o�֐�
	void Initialize();

	void TransConstBuffer();

	void SetLightDir(const XMVECTOR& lightdir);

	void SetLightColor(const XMFLOAT3& lightcolor);

	void Update();

	void Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParameterIndex);


};

