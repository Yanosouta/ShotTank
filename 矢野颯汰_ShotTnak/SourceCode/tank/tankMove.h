#pragma once
#include "component.h"
#include "gameObject.h"
#include "scene.h"
#include "player.h"
#include "manager.h"
#include "CreateBall.h"

#define TARGETPOS 12	//�ړI�n�̐�
#define ROWS 3			//�c
#define COLS 4			//��
#define DELAY 1.0f		//�čs���܂ł̎���

class TankMove : public Component
{
public:
	using Component::Component;

	void Init() override;	//������
	bool Navi();			//�ړ�
	//���ςŊp�x�����߂�
	float face(DirectX::XMFLOAT3);
	//�^���N�ƃv���C���[�̊ԂɃI�u�W�F�N�g�����邩����
	void Vision();
	//�z��̈ʒu�����߂�
	void ArrayPoint();
	//2�����z��̗אڂ����v�f�����߂�
	void SelectAdjacent(int row, int col);
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
	//�ړ��ԂɃI�u�W�F�N�g�����邩����
	void MovePossibility();

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
	Scene* scene = Manager::GetScene();					//�V�[���̏����擾
	Player* player = scene->GetGameObject<Player>();	//�v���C���[�̏����擾

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
	//�ړI�n
	DirectX::XMFLOAT3 m_TargetPos[12] = {
		{2.0f,0.0f,2.0f},		//0
		{11.0f,0.0f,2.0f},		//1
		{23.0f,0.0f,2.0f},		//2
		{33.0f,0.0f,2.0f},		//3
		{2.0f,0.0f,10.0f},		//4
		{11.0f,0.0f,10.0f},		//5
		{23.0f,0.0f,10.0f},		//6
		{33.0f,0.0f,10.0f},		//7
		{2.0f,0.0f,18.0f},		//8
		{11.0f,0.0f,18.0f},		//9
		{23.0f,0.0f,18.0f},		//10
		{33.0f,0.0f,18.0f},		//11
	};
	//�}�b�v���
	int m_TargetPosArray[ROWS][COLS] = {
		{8, 9,10,11},
		{4, 5, 6, 7},
		{0, 1, 2, 3},
	};
	//�אڂ����z����i�[
	int m_AbjacentPos[4] = { 0,0,0,0 };


	bool m_MoveFlg;						//�ړ�����t���O
	bool m_DirObj[4];					//�ړ���������t���O
	bool m_OneFlg = true;				//��x���s�t���O
	bool m_excFlg;						//�v�Z�t���O
	float m_Distance[TARGETPOS];		//����
	float m_Degrees;					//�x
	float m_RotY;						//�p�x
	float m_Delay;						//�x�����鎞��
	float m_DelayCount;					//�x���J�E���g
	float m_correctedShotTime = 0.0f;	//�����ɒe��ł��Ȃ��悤�ɕ␳����
	int m_randPos = 0;					//rand�̒l���i�[
	int m_FrameCount;					//�J�E���g
	
	DirectX::XMFLOAT3 m_Position;		//���W
	DirectX::XMFLOAT3 m_Forward;		//�O���x�N�g��
	DirectX::XMFLOAT3 m_Rotation;		//�p�x
	DirectX::XMFLOAT3 m_Speed = 		//���x
		DirectX::XMFLOAT3(0.1f, 0.1f, 0.1f);

	//�f�o�b�O�p
	bool m_DebugFlg = false;
	CreateBall* m_CBall[TARGETPOS] ;
	
};

