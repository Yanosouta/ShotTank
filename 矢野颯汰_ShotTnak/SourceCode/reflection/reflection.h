#pragma once
#include "component.h"
#include "gameObject.h"
#include "scene.h"
#include "box.h"
#include "bullet.h"
#include "pbullet.h"
class Reflection : public Component
{
public:
	using Component::Component;

	//===========�֐���`============
	void Init() override;
	void Update() override;

	bool UpCollision(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 scale);		//��̓����蔻��
	bool DownCollision(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 scale);		//���̓����蔻��
	bool LeftCollision(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 scale);		//���̓����蔻��
	bool RightCollision(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 scale);	//�E�̓����蔻��

	void SetBullet(Bullet*);		//�e�̏����擾
	void SetPBullet(PBullet*);		//�e�̏����擾
	
private:
	//--------�e�����擾--------
	Scene* m_Scene;
	std::vector <Box*> m_Box;
	Bullet* m_Bullet;
	PBullet* m_PBullet;
	
private:
	//==========�ϐ���`============
	//---------�{�b�N�X------------
	DirectX::XMFLOAT3 m_BoxPos;
	DirectX::XMFLOAT3 m_BoxScale;

	//-----------�e---------------
	DirectX::XMFLOAT3 m_BulletPos;
	DirectX::XMFLOAT3 m_BulletScale;
	DirectX::XMFLOAT3 m_BulletVel;

	DirectX::XMFLOAT3 m_TouchNor;		//�@�������i�[�p

	//�@�����
	DirectX::XMFLOAT3 m_Normal[6] = {
		{-1.0f,  0.0f,  0.0f},	// ��
		{ 1.0f,  0.0f,  0.0f},	// �E
		{ 0.0f, -1.0f,  0.0f},	// ��
		{ 0.0f,  1.0f,  0.0f},	// ��
		{ 0.0f,  0.0f, -1.0f},	// �O
		{ 0.0f,  0.0f,  1.0f},	// ��
	};
	//����
	int m_ReflCount;		//���˂̉񐔃J�E���g
	int m_Refl;				//���ˉ�
};

