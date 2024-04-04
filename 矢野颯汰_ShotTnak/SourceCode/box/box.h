// Box�I�u�W�F�N�g [box.h]
#pragma once
#include "gameObject.h"
#include "bullet.h"
#include "scene.h"
class Box : public GameObject
{
public:
	void Init() override;
	void Update() override;
private:
	// ���_���
	struct Vertex
	{
		DirectX::XMFLOAT3	pos;		// ���W
		DirectX::XMFLOAT3	normal;		// �@��
		DirectX::XMFLOAT2	uv;			// �e�N�X�`�����W
		float				weight[4];	// ���_�u�����h�E�F�C�g
		unsigned int		index[4];	// ���_�u�����h�C���f�b�N�X
	};
	DirectX::XMFLOAT3 g_Normal[6] = {
		{-1.0f,  0.0f,  0.0f},	// ��
		{ 1.0f,  0.0f,  0.0f},	// �E
		{ 0.0f, -1.0f,  0.0f},	// ��
		{ 0.0f,  1.0f,  0.0f},	// ��
		{ 0.0f,  0.0f, -1.0f},	// �O
		{ 0.0f,  0.0f,  1.0f},	// ��
	};
};


