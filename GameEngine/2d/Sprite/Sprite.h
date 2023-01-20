#pragma once
#include<Windows.h>
#include<wrl.h>
#include<d3d12.h>
#include<d3dx12.h>
#include<DirectXMath.h>

class Sprite
{
private: // �G�C���A�X
// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public://�T�u�N���X

	struct VertexPosUv
	{
		XMFLOAT3 pos;
		XMFLOAT2 uv;
	};

	struct ConstBufferData
	{
		XMFLOAT4 color;//�F
		XMMATRIX mat;//3D�ϊ��s��
	};

public://�ÓI�����o�֐�

	static bool StaticInitialize(ID3D12Device* device, int window_width, int window_height);

	static bool LoadTexture(UINT texNumber, const wchar_t* filename);

	static void PreDraw(ID3D12GraphicsCommandList* cmdList);

	static void PostDraw();

	static Sprite* SpriteCreate(UINT texNumber, XMFLOAT2 position, XMFLOAT4 color = { 1, 1, 1, 1 }, XMFLOAT2 anchorpoint = { 0.0f, 0.0f }, bool isFlipX = false, bool isFlipY = false);

protected://�ÓI�����o�ϐ�
	static const int spriteSRVCount = 512;
	//���_��
	static const int vertNum = 4;
	//�f�o�C�X
	static ID3D12Device* dev;
	// �f�X�N���v�^�T�C�Y
	static UINT descriptorHandleIncrementSize;
	//�R�}���h���X�g
	static ID3D12GraphicsCommandList* cmdList;
	//�p�C�v���C��
	static ComPtr<ID3D12PipelineState>pipelinestate;
	//���[�g�V�O�l�`���[
	static ComPtr<ID3D12RootSignature>rootsignature;
	//�ˉe�s��
	static XMMATRIX matProjection;
	//�e�N�X�`���p�f�X�N���v�^�q�[�v�̐���
	static ComPtr<ID3D12DescriptorHeap> descHeap;
	//�e�N�X�`�����\�[�X(�e�N�X�`��)�̔z��
	static ComPtr<ID3D12Resource>texBuff[spriteSRVCount];
	//	//�萔�o�b�t�@�p�̃f�X�N���v�^�q�[�v
	static ComPtr<ID3D12DescriptorHeap> basicDescHeap;

public://�����o�֐�

	Sprite(UINT texnumber,XMFLOAT2 position, XMFLOAT2 size, XMFLOAT4 color, XMFLOAT2 anchorpoint, bool FlipX, bool FlipY);

	//������
	bool InitializeSprite();
	//bool SpriteCreateGraaphicsPipeline(ID3D12Device* dev);
	//��]
	void SetRotation(float rotation);
	//���W
	void SetPosition(XMFLOAT2 position);
	//�T�C�Y
	void SetSize(XMFLOAT2 size);
	//�A���J�[�|�C���g
	void SetAnchor(XMFLOAT2 anchorpoint);
	//���E���]
	void SetFlipX(bool FlipX);
	//�㉺���]
	void SetFlipY(bool FlipY);
	//�e�N�X�`��
	void SetTexRect(XMFLOAT2 texBase, XMFLOAT2 texSize);
	//�`��
	void Draw();

protected://�����o�ϐ�
		//���_�o�b�t�@
	ComPtr<ID3D12Resource> vertBuff ;
	//�萔�o�b�t�@
	ComPtr<ID3D12Resource>constBuff ;
	//���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	
	//�e�N�X�`���ԍ�
	UINT texNumber = 0;
	
	//Z�����̉�]�p
	float rotation = 0.0f;
	//���W
	XMFLOAT2 position{0.0f,0.0f};
	//�傫��
	XMFLOAT2 size = { 1.0f,1.0f };
	//���[���h�s��
	XMMATRIX matWorld{};
	//�F(RGBA)
	XMFLOAT4 color = { 1,1,1,1 };
	//���E�㉺���]
	bool FlipX = false;
	bool FlipY = false;
	//�A���J�[�|�C���g
	XMFLOAT2 anchorpoint = { 0,0 };
	//�e�N�X�`���n�_
	XMFLOAT2 texBase = { 0,0 };
	//�e�N�X�`����
	XMFLOAT2 texSize = { 1.0f,1.0f };
private:
	void Trans();
};

