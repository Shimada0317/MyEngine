#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include<string>

using namespace std;
using namespace DirectX;
using namespace Microsoft::WRL;

class ObjModel
{
public: // �T�u�N���X
// ���_�f�[�^�\����
	struct VertexPosNormalUv
	{
		DirectX::XMFLOAT3 pos; // xyz���W
		DirectX::XMFLOAT3 normal; // �@���x�N�g��
		DirectX::XMFLOAT2 uv;  // uv���W
	};

	struct Material
	{
		std::string name;
		DirectX::XMFLOAT3 ambient;
		DirectX::XMFLOAT3 diffuse;
		DirectX::XMFLOAT3 specular;
		float alpha;
		std::string textureFilename;
		//�R���X�g���N�g
		Material() {
			ambient = { 0.3f,0.3f,0.3f };
			diffuse = { 0.0f,0.0f,0.0f };
			specular = { 0.0f,0.0f,0.0f };
			alpha = 1.0f;
		}
	};

	struct ConstBufferDataB1
	{
		DirectX::XMFLOAT3 ambient;//�A���r�G���g
		float pad1;//�p�f�B���O
		DirectX::XMFLOAT3 diffuse;//�f�B�t���[�Y
		float pad2;//�p�f�B���O
		DirectX::XMFLOAT3 specular;//�X�y�L�����[
		float alpha;//�A���t�@
	};
public:
	//OBJ�t�@�C���ǂݍ���
	static ObjModel* LoadFromObJ(const std::string& modelname);
	static void SetDev(ID3D12Device* dev) { ObjModel::dev = dev; }
private://�����o�ϐ�
	//�C���f�b�N�X�z��
	std::vector<unsigned short> indices;
	// ���_�o�b�t�@
	ID3D12Resource* vertBuff;
	// �C���f�b�N�X�o�b�t�@
	ID3D12Resource* indexBuff;
	// ���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView;
	// �C���f�b�N�X�o�b�t�@�r���[
	D3D12_INDEX_BUFFER_VIEW ibView;
	// ���_�f�[�^�z��
	//static VertexPosNormalUv vertices[vertexCount];
	std::vector<VertexPosNormalUv> vertices;
	//�萔�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource>constBuffB1;
	//�}�e���A��
	Material material;
	// �f�X�N���v�^�T�C�Y
	UINT descriptorHandleIncrementSize;
	// �f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeap;
	// �V�F�[�_���\�[�X�r���[�̃n���h��(CPU)
	CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
	// �V�F�[�_���\�[�X�r���[�̃n���h��(CPU)
	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;
	// �e�N�X�`���o�b�t�@
	ComPtr<ID3D12Resource> texbuff;

private://����J�̃����o�֐�
	void LoadFromOBJInternal(const std::string&  modelname);
public://�����o�֐�
	//�}�e���A���ǂݍ���
	void LoadMaterial(const std::string& directoryPath, const std::string& filename);
	//�e�N�X�`���ǂݍ���
	bool LoadTexture(const std::string& directoryPath, const std::string& filename);
	//�f�X�N���v�^�q�[�v�̏�����
	void InitializeDescriptorHeap();
	//�e��o�b�t�@����
	void CreateBuffers();
	//�`��
	void Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParamIndexMaterial);
private://�ÓI�����o�ϐ�
	//�f�o�C�X
	static ID3D12Device* dev;
};

