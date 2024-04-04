// pBullet�I�u�W�F�N�g [pbullet.h]
#pragma once
#include "gameObject.h"
#include "component.h"
#include "scene.h"


class PBullet : public GameObject
{
private:
	Scene* scene;
	float m_length = 0.0f;

public:
	void Init() override;	//������
	void Update() override;	//�X�V
	
	//�x�N�g�����Z�b�g
	void SetVelocity(DirectX::XMFLOAT3 Velocity) { m_Velocity = Velocity; }
	//�x�N�g�����擾
	DirectX::XMFLOAT3 GetVelocity() { return m_Velocity; }

	
	//�F��ς���
	void ChangeColor();


};

