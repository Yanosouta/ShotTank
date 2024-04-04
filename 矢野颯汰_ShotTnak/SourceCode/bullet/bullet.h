// Bullet�I�u�W�F�N�g [bullet.h]
#pragma once
#include "gameObject.h"
#include "scene.h"
#include "player.h"
class Bullet : public GameObject
{
public:
	void Init() override;		//������
	void Update() override;		//�X�V
	//�����蔻��
	void collision();	
	//�e�̃x�N�g�����Z�b�g
	void SetVelocity(DirectX::XMFLOAT3 Velocity) { m_Velocity = Velocity; }	
	//�e�̃x�N�g�����擾
	DirectX::XMFLOAT3 GetVelocity() { return m_Velocity; }
	
private:
	Scene* scene;
	Player* player;
private:

	float m_length = 0.0f;//����

};

