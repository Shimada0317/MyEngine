#pragma once
#include"Object3d.h"
#include"ObjModel.h"
#include"ModelManager.h"
#include<DirectXMath.h>
#include"Texture.h"

class ObjParticle
{
private: // �G�C���A�X
// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;
public:
	~ObjParticle();

	/// <summary>
	/// ����������
	/// </summary>
	/// <param name="ModelNumber">���f���}�l�[�W���[��</param>
	/// <param name="particlepos">�p�[�e�B�N�������n�_</param>
	/// <param name="particlescl">�p�[�e�B�N���̏����T�C�Y</param>
	/// <param name="particlerot">�p�[�e�B�N���̌���</param>
	void Initialize(int ModelNumber,const XMVECTOR& particlepos,const XMFLOAT3& particlescl,const XMFLOAT3& particlerot);
	/// <summary>
	/// �X�e�[�^�X�Z�b�g
	/// </summary>
	void Set();
	/// <summary>
	/// �X�V����
	/// </summary>
	void Update(int smole=200);
	/// <summary>
	/// �`�揈��
	/// </summary>
	void Draw();
	/// <summary>
	/// �I������
	/// </summary>
	void Finalize();

	/// <summary>
	/// ��������
	/// </summary>
	/// <returns>�����t���O</returns>
	bool IsDelete()const { return Delete_; }
private:

	std::unique_ptr<Object3d>Particle;
	//�p�[�e�B�N���X�e�[�^�X
	XMVECTOR ParticlePos = { 0.0f,0.0f,0.0f };
	XMFLOAT3 ParticleScl = { 0.0f,0.0f,0.0f };
	XMFLOAT3 ParticleRot = { 0.0f,0.0f,0.0f };
	//�����t���O
	bool Delete_;
	//�����_���p�ϐ�
	float RandomZ;
	float RandomY;
	float RandomX;
	float RandomRot;
	float SmoleScl;
	//����
	float time = 0.0f;
	//�d��
	float gravity = 2.0f;
};

