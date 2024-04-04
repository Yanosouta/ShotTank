#pragma once
#include "component.h"
#include "gameObject.h"
#include "scene.h"
#include "player.h"
#include "manager.h"
#include "AStar.h"
#include "CreateBall.h"


#define MAP 153		//�ړI�n�̐�

class AStarMove :public Component
{
public:
	using Component::Component;
	//������
	void Init() override;	
	//�v���C���[�܂ňړ�
	bool Navi();			
	//���ςŊp�x�����߂�
	float face(DirectX::XMFLOAT3);
	//��ԋ߂��|�C���g�����߂�
	DirectX::XMINT2 NearPoint(DirectX::XMFLOAT3 pos);
	//�^���N�ƃv���C���[�̊ԂɃI�u�W�F�N�g�����邩����
	void Vision();
	//���������߂�
	float Dist(DirectX::XMFLOAT3 pos1, DirectX::XMFLOAT3 pos2) {
		float dx = pos2.x - pos1.x;
		float dz = pos2.z - pos1.z;
		return sqrt(dx * dx + dz * dz);
	}
	//���W�A������x�ɕϊ�
	float ToDegrees(float rad) { return rad * (180.0f / 3.1415f); }
	//�x���烉�W�A���ɕϊ�
	float ToRadian(float degrees) { return degrees * (3.1415f / 180.0f); }
	
	//�f�o�b�O�p
	//�F��ς���
	void ChangeColor(CreateBall* ball) {
		ball->ChangeColor();
	}
	//�F�������̐F�ɖ߂�
	void ReturnColor(CreateBall* ball) {
		ball->ReturnColor();
	}
private:
	//�@��
	DirectX::XMFLOAT3 m_Normal[6] = {
		{-1.0f,  0.0f,  0.0f},
		{ 1.0f,  0.0f,  0.0f},
		{ 0.0f, -1.0f,  0.0f},
		{ 0.0f,  1.0f,  0.0f},
		{ 0.0f,  0.0f, -1.0f},
		{ 0.0f,  0.0f,  1.0f},
	};


private:
	Scene* scene = Manager::GetScene();	//�V�[��
	Player* player;		//�v���C���[
	
	AStar* astar;		//AStar
	CreateBall* m_CBall[MAP_HEIGHT][MAP_WIDTH];	//�f�o�b�O�p�{�[��

private:
	//�ϐ���`
	bool m_excFlg;				//������������t���O
	bool m_OneFlg = true;		//��]���邩����t���O
	bool m_MoveOneFlg = true;	//�ړ����Ɉ�x�������s
	bool m_InitOneFlg = true;	//�����ݒ�t���O
	bool m_AStarFlg = true;		//AStar�̌v�Z���s��������t���O
	bool m_StateFlg = true;		//�X�e�[�g��J�ڂ����邩����t���O

	float m_Degrees;					//�x
	float m_RotY;						//�p�x
	float m_correctedShotTime = 0.0f;	//�����ɒe��ł��Ȃ��悤�ɕ␳����

	//�}�b�v
	DirectX::XMINT2 m_MapIndex;							//�z��̓Y�������i�[
	DirectX::XMFLOAT2 m_MapPos[MAP_HEIGHT][MAP_WIDTH];	//�ړI�n�̍��W���i�[(FLOAT2)
	DirectX::XMFLOAT3 m_PointPos;						//�ړI�n�̍��W���i�[(FLOAT3)

	DirectX::XMFLOAT3 m_Position;						//���W
	DirectX::XMFLOAT3 m_Forward;						//�O���x�N�g��
	DirectX::XMFLOAT3 m_Rotation;						//�p�x
	DirectX::XMFLOAT3 m_Speed =							//�ړ����x
		DirectX::XMFLOAT3(0.1f, 0.1f, 0.1f);

	//list
	std::list<DirectX::XMINT2> m_astarList;		//�v���C���[�܂ł̃m�[�h���i�[

	//�f�o�b�O�p
	bool m_DebugFlg = false;
};


