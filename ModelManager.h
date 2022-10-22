#pragma once


class ModelManager final
{
private:
	//�R���X�g���N�^��private�ɂ���
	ModelManager();
	//�f�X�g���N�^��private�ɂ���
	~ModelManager();
public:
	//�R�s�[�R���X�g���N�^�𖳌��ɂ���
	ModelManager(const ModelManager& model) = delete;
	//������Z�q�𖳌��ɂ���
	ModelManager& operator=(const ModelManager& model) = delete;

	static ModelManager* GetInstance();
};

