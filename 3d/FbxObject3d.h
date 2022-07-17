#pragma once

#include"FbxModel.h"
#include"Camera.h"

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include<string>

class FbxObject3d
{
public://�萔
//�{�[���̍ő吔
	static const int MAX_BONES = 32;
protected:
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
public://�T�u�N���X
	struct ConstBufferDataTransForm
	{
		XMMATRIX viewproj;
		XMMATRIX world;
		XMFLOAT3 camerapos;
	};

	struct ConstBufferDataSkin
	{
		XMMATRIX bones[MAX_BONES];
	};
public://�ÓI�����o
	static void SetDevice(ID3D12Device* device) { FbxObject3d::device = device; }
	static void SetCamera(Camera* camera) { FbxObject3d::camera = camera; }
public://�����o�֐�
/// <summary>
/// ������
/// </summary>
	void Initialize();

/// <summary>
/// �O���t�B�b�N�X�p�C�v���C���̐���
/// </summary>
	static void CreateGraphicPipeline();
/// <summary>
/// ���t���[������
/// </summary>
	void Update();
	/// <summary>
/// �`��
/// </summary>
	void Draw(ID3D12GraphicsCommandList* cmdList);

/// <summary>
/// ���f���̃Z�b�g
/// </summary>
	void SetModel(FbxModel* model) { this->model = model; }
/// <summary>
/// �p�x�̃Z�b�g
/// </summary>
	void SetRotation(XMFLOAT3 rotation) { this->rotation = rotation; }

	/// <summary>
/// �p�x�̃Z�b�g
/// </summary>
	void SetPosition(XMFLOAT3 position) { this->position = position; }

/// <summary>
/// �A�j���[�V�����J�n
/// </summary>
	void PlayAnimation();

protected://�����o�ϐ�
	ComPtr<ID3D12Resource>ConstBufferTransForm;

	static ComPtr<ID3D12RootSignature> rootsignature;

	static ComPtr<ID3D12PipelineState> pipelinestate;

	ComPtr<ID3D12Resource> constBuffSkin;
private:
	static ID3D12Device* device;

	static Camera* camera;

protected:
	XMFLOAT3 scale = { 1,1,1 };

	XMFLOAT3 rotation = { 0,0,0 };

	XMFLOAT3 position = { 0,0,0 };

	XMMATRIX matWorld;

	FbxModel* model = nullptr;
public:
	//1�t���[���̎���
	FbxTime frameTime;
	//�A�j���[�V�����J�n����
	FbxTime startTime;
	//�A�j���[�V�����I������
	FbxTime endTime;
	//���ݎ���
	FbxTime currentTime;
	//�A�j���[�V�����Đ���
	bool isPlay = false;
};

