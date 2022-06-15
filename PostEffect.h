#pragma once
#include "Sprite.h"
#include<Windows.h>
#include<wrl.h>
#include<d3d12.h>
#include<d3dx12.h>

class PostEffect :
    public Sprite
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
    ///<summary>
    ///�R���X�g���N�^
    ///</summary>
    PostEffect();

    ///<summary>
    ///������
    ///</summary>
    void Initialize();

    ///<summary>
    ///�`��
    ///</summary>
    ///<param name="cmdList">�R�}���h���X�g</param>
    void Draw(ID3D12GraphicsCommandList* cmdList);

    ///<summary>
   ///�`��O����
   ///</summary>
   ///<param name="cmdList">�R�}���h���X�g</param>
    void PreDrawScene(ID3D12GraphicsCommandList* cmdList);

   ///<summary>
   ///�`��㏈��
   ///</summary>
   ///<param name="cmdList">�R�}���h���X�g</param>
   void PostDrawScene(ID3D12GraphicsCommandList* cmdList);

private:
    //�e�N�X�`���o�b�t�@
    ComPtr<ID3D12Resource>texBuff;
    //SRV�p�̃f�X�N���v�^�q�[�v
    ComPtr<ID3D12DescriptorHeap>descHeapSRV;
    //�[�x�o�b�t�@
    ComPtr<ID3D12Resource> depthBuff;
    //RTV(�����_�[�^�[�Q�b�g)�p�f�X�N���v�^�q�[�v
    ComPtr<ID3D12DescriptorHeap> descHeapRTV;
    //DSV(�f�v�X�e���V���r���[)�p�f�X�N���v�^�q�[�v
    ComPtr<ID3D12DescriptorHeap> descHeapDSV;

private://�ÓI�����o�ϐ�
    //��ʃN���A�J���[
    static const float clearColor[4];
};

