#pragma once
#include "component.h"
#include "scene.h"
#include "pbullet.h"
class TankAvoid :public Component
{
private:
	Scene* scene;					//�V�[��
	std::vector<PBullet*> pbullet;	//�e
	//�����肻���Ȓe�̏��
	struct HitBullet
	{
		bool hit;						//�����肻��������
		DirectX::XMFLOAT3 pos;			//���W
		DirectX::XMFLOAT3 velo;			//�x�N�g��
		float dot;						//���ς̌���
	};
	HitBullet m_HitBullet;				//�\���̐錾
	DirectX::XMFLOAT3 m_Position;		//���W
	DirectX::XMFLOAT3 m_Forward;		//�O���x�N�g��
	DirectX::XMFLOAT3 m_Rotation;		//�p�x
	DirectX::XMFLOAT3 m_Speed;			//���x
	DirectX::XMFLOAT3 m_RayDir[4];		//���C�̔�΂�����
	DirectX::XMFLOAT3 m_Dir4[4] = {		//4����
		{-1.0f,  0.0f,  0.0f},
		{ 1.0f,  0.0f,  0.0f},
		{ 0.0f,  0.0f, -1.0f},
		{ 0.0f,  0.0f,  1.0f},
	};
	DirectX::XMFLOAT3 m_FacePoint[4] = {	//���C�̓����������W
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f} };
	DirectX::XMFLOAT3 m_Normal[6] = {		//�@��
		{-1.0f,  0.0f,  0.0f},
		{ 1.0f,  0.0f,  0.0f},
		{ 0.0f, -1.0f,  0.0f},
		{ 0.0f,  1.0f,  0.0f},
		{ 0.0f,  0.0f, -1.0f},
		{ 0.0f,  0.0f,  1.0f},
	};
	float m_Vec[4] = { 0.0f,0.0f,0.0f,0.0f };	//�x�N�g��
	bool m_AvoidFlg;					//�����锻��t���O
	bool m_CollisionFlg;				//�I�u�W�F�N�g�ɓ���������true
	float m_PAngle;						//���W�܂ł̊p�x
	float m_tempAngle = 0.0f;			//����ۊ�
	float m_avertDelay;					//�ēx�������悤�ɂȂ�܂ł̎���
	float m_avertCount;					//�J�E���g
	float m_MoveTime ;			//�����鎞�̈ړ����鎞��
	float m_MoveCount;
	
	//�f�o�b�O�p
	bool DebugFlg = false;
	bool DebugOneFlg = true;
	float DebugTime ;
	DirectX::XMFLOAT3 DebugPos;
public:
	using Component::Component;
	
	void Init();	//������
	//������
	bool Avoid();	
	//���������ʂ܂ł̒������\�[�g
	void LenSort();
	//�O�ς��g�����p�x�̌v�Z
	float PointAngle();
	//�e�������肻��������
	void Possibility();
	//���ςŊp�x�����߂�
	float face(DirectX::XMFLOAT3);
	//���ς̌v�Z
	HitBullet Dot(DirectX::XMFLOAT3 vec, DirectX::XMFLOAT3 pos);
};

