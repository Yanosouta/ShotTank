#include "tankMove.h"
#include "ray.h"
#include "collision.h"
#include "box.h"
#include "gameObject.h"
#include "scene.h"
#include <list>
//=======================
//		������
//=======================
void TankMove::Init()
{
	
	m_MoveFlg = true;		
	m_Delay = DELAY;
	m_DelayCount = DELAY;  
		
	//�f�o�b�O���I���̎��ړI�n��\��
	if (m_DebugFlg)
	{
		for (int i = 0; i < TARGETPOS; i++)
		{
			m_CBall[i] = scene->GetGameObject<CreateBall>();
			m_CBall[i] = scene->AddGameObject<CreateBall>();
			m_CBall[i]->SetPosition(m_TargetPos[i]);
			m_CBall[i]->SetScale(DirectX::XMFLOAT3(2.0f, 2.0f, 2.0f));
		}
	}
}
//=======================
//		�@�ړ�
//=======================
bool TankMove::Navi()
{
	//�ϐ���`
	float dist;
	bool m_StateFlg = false;
	//�e�����擾
	m_Position = m_GameObject->GetPosition();
	m_Rotation = m_GameObject->GetRotation();
	m_Forward = m_GameObject->GetForward();
	
	

	//�ړI�n�܂ł̋��������߂�
	if (m_MoveFlg)
	{
		m_MoveFlg = false;
		//�e�ړI�n�܂ł̋������v�Z
		for (int i = 0; i < TARGETPOS; i++)
		{
			m_Distance[i] = Dist(m_TargetPos[i], m_Position);
		}
		//��ԋ߂��ړI�n�����߂�
		ArrayPoint();
		//4�����ŃI�u�W�F�N�g�����邩���肷��
		MovePossibility();
		//�I�u�W�F�N�g����������͏��O
		for (int i = 0; i < 4; i++)
		{
			//�I�u�W�F�N�g����������ɂ�-1
			if (m_DirObj[i])
				m_AbjacentPos[i] = -1;
		}
		//�I�u�W�F�N�g�����������ɂȂ�܂ŉ�
		while (1)
		{
			m_randPos = rand() % 4;

			//�����_���̌��ʊi�[���ꂽ�ϐ��������Ȃ�while�����ʂ���
			if (m_AbjacentPos[m_randPos] != -1)
				break;
		}

	}
	//�ړI�n�܂ł̋�������ɍX�V
	dist = Dist(m_TargetPos[m_AbjacentPos[m_randPos]], m_Position);
	//�ړI�n�܂ł̊p�x���v�Z
	m_RotY = face(m_TargetPos[m_AbjacentPos[m_randPos]]);
	//�x�ɕϊ�
	m_Degrees = ToDegrees(m_RotY);
	//�ړI�n�̐F��ς���i�f�o�b�O�p�j
	if(m_DebugFlg)
		ChangeColor(m_CBall[m_AbjacentPos[m_randPos]]);
	//�o�O��Ȃ��悤��1�x�����␳
	if (m_OneFlg)
	{
		if (m_Degrees >= 0.0f) {
			m_Degrees += 1.5f;
		}
		else {
			m_Degrees -= 1.5f;
		}
		m_OneFlg = false;
	}
	//�ړI�n�̕����ɉ�]
	if (m_Degrees >= 1.5f || m_Degrees <= -1.5f)
	{
		if (m_Degrees >= 0.0f) {
			m_Rotation.y += ToRadian(1.5f);
		}
		else {
			m_Rotation.y -= ToRadian(1.5f);
		}
	}
	else {
		//�ړ�
		if (m_DelayCount > m_Delay) {
			if (m_TargetPos[m_AbjacentPos[m_randPos]].x > 0.1f)
				m_Position.x += m_Forward.x * m_Speed.x;
			if (m_TargetPos[m_AbjacentPos[m_randPos]].z > 0.1f)
				m_Position.z += m_Forward.z * m_Speed.z;
		}
	}
	//�ړI�n�ɒ�������t���O�����Z�b�g
	if (dist < 0.2f)
	{
		//�ړI�n�̐F��߂��i�f�o�b�O�p�j
		if (m_DebugFlg)
			ReturnColor(m_CBall[m_AbjacentPos[m_randPos]]);
		//������
		m_MoveFlg = true;
		m_OneFlg = true;
		m_DelayCount = 0.0f;
	}
	//�^���N�ƃv���C���[�̊ԂɃI�u�W�F�N�g�����邩����
	Vision();	
	//�ԂɃI�u�W�F�N�g���Ȃ���Β��ɓ���
	if (!m_excFlg)
	{
		m_correctedShotTime += 1.0f / 60.0f;
		if (m_correctedShotTime >= 0.5f)
		{
			//�J�ڂ���O�Ƀt���O��������
			//�ړI�n�̐F��߂��i�f�o�b�O�p�j
			if (m_DebugFlg)
				ReturnColor(m_CBall[m_AbjacentPos[m_randPos]]);

			//������
			m_correctedShotTime = 0.0f;
			m_MoveFlg = true;
			m_OneFlg = true;
			m_DelayCount = 0.0f;
			//�J�ڃt���O��true
			m_StateFlg = true;
		}
		
	}
	m_DelayCount += 1.0f / 60.0f;
	//�����Z�b�g
	m_GameObject->SetPosition(m_Position);
	m_GameObject->SetRotation(m_Rotation);
	return m_StateFlg;
}
//=======================
//	���ςŊp�x�����߂�
//=======================
float TankMove::face(DirectX::XMFLOAT3 targetPos)
{
	//=====�ϐ���`=====
	DirectX::XMFLOAT3 TargetDir;	//�ړI�n�܂ł̋���
	DirectX::XMVECTOR DirVector;	//���K����������
	float rotY = 0.0f;				//�p�x
	DirectX::XMFLOAT3 forward = m_Forward;	//�O���x�N�g��
	//�ʒu�x�N�g���̌v�Z
	TargetDir = DirectX::XMFLOAT3(targetPos.x - m_Position.x, targetPos.y - m_Position.y, targetPos.z - m_Position.z);
	//���K��
	DirVector = DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&TargetDir));
	//���ς̌v�Z
	//90���␳
	DirectX::XMMATRIX  rotMat = DirectX::XMMatrixRotationY(3.14f / 2);
	DirectX::XMStoreFloat3(&forward, DirectX::XMVector3Normalize(DirectX::XMVector3Transform(DirectX::XMLoadFloat3(&forward), rotMat)));

	//���ς̌v�Z
	DirectX::XMStoreFloat(&rotY, DirectX::XMVector3Dot(
		DirectX::XMVector3Normalize(DirVector),
		DirectX::XMLoadFloat3(&forward)));

	return rotY;
}
//=========================================================
//		�^���N�ƃv���C���[�̊ԂɃI�u�W�F�N�g�����邩����
//=========================================================
void TankMove::Vision()
{
	//======�ϐ���`======
	Ray::Result result;			//Ray�̌��ʂ��i�[
	Ray::Result exclusion;		//Ray�̌��ʂ��i�[
	Ray::Triangle triangle[2];	//���_�����i�[
	Collision::Vertices vtx;	//���_���
	DirectX::XMFLOAT3 point;	//Ray�̓����������W
	m_excFlg = false;

	DirectX::XMFLOAT3  playerPos =	//�v���C���[�̍��W
		player->GetPosition();
	DirectX::XMFLOAT3 TargetDir =	//�v���C���[�ƃ^���N�̋���
		DirectX::XMFLOAT3(playerPos.x - m_Position.x,
			playerPos.y - m_Position.y,
			playerPos.z - m_Position.z);
	//�x�N�g���̌v�Z
	float Dis = sqrtf(TargetDir.x * TargetDir.x + TargetDir.z * TargetDir.z);
	//�v���C���[�܂Ń��C���΂�
	for (int i = 0; i < 6; i++)
	{
		//Y�������O
		if (i != 2 && i != 3)
		{
			//�V�[�����ɂ���Box�̏����擾
			std::vector<Box*> boxList = scene->GetGameObjects<Box>();
			//Box�̐�������
			for (Box* box : boxList)
			{
				//�@���̐�������
				for (int j = 0; j < 6; j++)
				{
					DirectX::XMFLOAT3 boxPos = box->GetPosition();	//���W
					DirectX::XMFLOAT3 boxSize = box->GetScale();	//�T�C�Y
					//�@���̕����̃T�C�Y��������
					if (j == 0)
						boxPos.x -= boxSize.x;	//-X����
					if (j == 1)
						boxPos.x += boxSize.x;	// X����
					if (j == 4)
						boxPos.z -= boxSize.z;	//-Z����
					if (j == 5)
						boxPos.z += boxSize.z;	// Z����
					//�@������Y�������O
					if (j != 2 && j != 3)
					{
						//���C�̌v�Z
						exclusion = Ray::CheckRayPlane(m_Normal[j], boxPos, m_Position, TargetDir, Dis + 0.1f);
						//���C�����������ʂ������true
						if (exclusion.hit)
						{
							//���_�����擾
							vtx = box->GetComponent<Collision>()->GetVertices();
							//�@���e�ʂ̒��_�����i�[
							switch (j)
							{		//�O�p�`2�łP�̖�
							case 0:
								triangle[0] = { vtx.pos[0],vtx.pos[2],vtx.pos[4] };
								triangle[1] = { vtx.pos[2],vtx.pos[6],vtx.pos[4] };
								break;
							case 1:
								triangle[0] = { vtx.pos[3],vtx.pos[1],vtx.pos[7] };
								triangle[1] = { vtx.pos[1],vtx.pos[5],vtx.pos[7] };
								break;
							case 4:
								triangle[0] = { vtx.pos[2],vtx.pos[3],vtx.pos[6] };
								triangle[1] = { vtx.pos[3],vtx.pos[7],vtx.pos[6] };
								break;
							case 5:
								triangle[0] = { vtx.pos[1],vtx.pos[0],vtx.pos[5] };
								triangle[1] = { vtx.pos[0],vtx.pos[4],vtx.pos[5] };
								break;
							default:
								break;
							}
							//���C�������������W��ۊ�
							point = exclusion.point;
							//�O�p�`�Q��
							for (int j = 0; j < 2; j++)
							{
								//�ʂ͈̔͊O�Ŕ��肵�Ȃ��悤�ɖʂ͈͓̔��Ȃ̂�����
								result = Ray::CheckPointTraiangle(point, triangle[j]);
								//�͈͓��ł����true
								if (result.hit)
								{
									//�͈͓��ł���Εʂ̕����܂ŃX�L�b�v
									m_excFlg = true;
									break;
								}
							}
							if (m_excFlg)
								break;
						}
					}

				}
				if (m_excFlg)
					break;
			}
			if (m_excFlg)
				break;
		}
	}
}
//==========================================
//�z��̕ϐ�����2�����z��̗v�f�����߂�
//==========================================
void TankMove::ArrayPoint()
{
	int point = 0;
	//�e�ړI�n�܂ł̋��������߂�
	for (int i = 0; i < TARGETPOS; i++)
	{
		if (m_Distance[i] <= 5.5f)
		{
			point = i;
			break;
		}
	}
	//��ԋ߂����W���N�_�ɂ���
	switch (point)
	{//�אڂ����v�f�����߂�
	case 0:
		
		SelectAdjacent(2, 0);
		break;
	case 1:
		SelectAdjacent(2, 1);
		break;
	case 2:
		SelectAdjacent(2, 2);
		break;
	case 3:
		SelectAdjacent(2, 3);
		break;
	case 4:
		SelectAdjacent(1, 0);
		break;
	case 5:
		SelectAdjacent(1, 1);
		break;
	case 6:
		SelectAdjacent(1, 2);
		break;
	case 7:
		SelectAdjacent(1, 3);
		break;
	case 8:
		SelectAdjacent(0, 0);
		break;
	case 9:
		SelectAdjacent(0, 1);
		break;
	case 10:
		SelectAdjacent(0, 2);
		break;
	case 11:
		SelectAdjacent(0, 3);
		break;
	default:
		break;
	}
}
//=======================
//	�אڂ����v�f�����߂�
//=======================
void TankMove::SelectAdjacent(int row, int col)
{
	//�����
	if (row - 1 > -1) {
		m_AbjacentPos[0] = m_TargetPosArray[row - 1][col];
	}
	else {
		m_AbjacentPos[0] = -1;
	}
	//������
	if (row + 1 < ROWS) {
		m_AbjacentPos[1] = m_TargetPosArray[row + 1][col];
	}
	else {
		m_AbjacentPos[1] = -1;
	}
	//�E����
	if (col + 1 < COLS) {
		m_AbjacentPos[2] = m_TargetPosArray[row][col + 1];
	}
	else {
		m_AbjacentPos[2] = -1;
	}
	//������
	if (col - 1 > -1) {
		m_AbjacentPos[3] = m_TargetPosArray[row][col - 1];
	}
	else {
		m_AbjacentPos[3] = -1;
	}
}
//========================================
//	�ړI�n�̕����ɃI�u�W�F�N�g������������
//========================================
void TankMove::MovePossibility()
{
	//�ϐ���`
	Ray::Result result;
	Ray::Result exclusion;
	Ray::Triangle triangle[2];
	Collision::Vertices vtx;
	DirectX::XMFLOAT3 point;
	float rotY = 0.0f;
	
	//4��������
	for (int i = 0; i < 4; i++)
	{
		bool checkFlg = false;

		//�ړI�n�܂ł̋���
		DirectX::XMFLOAT3 TargetDir =	
			DirectX::XMFLOAT3(m_TargetPos[m_AbjacentPos[i]].x - m_Position.x, 
								m_TargetPos[m_AbjacentPos[i]].y - m_Position.y, 
								m_TargetPos[m_AbjacentPos[i]].z - m_Position.z);
		//�x�N�g���̌v�Z
		float Dis = sqrtf(TargetDir.x * TargetDir.x + TargetDir.z * TargetDir.z);
		
		//-1�̏ꍇ�͏��O
		if (m_AbjacentPos[i] == -1)
			continue;
		//box�̏����擾
		std::vector<Box*> boxList = scene->GetGameObjects<Box>();
		//box�����镪��
		for (Box* box : boxList)
		{
			//�@���̐�������
			for (int k = 0; k < 6; k++)
			{

				DirectX::XMFLOAT3 boxPos = box->GetPosition();
				DirectX::XMFLOAT3 boxSize = box->GetScale();
				if (k == 0)
					boxPos.x -= boxSize.x;
				if (k == 1)
					boxPos.x += boxSize.x;
				if (k == 4)
					boxPos.z -= boxSize.z;
				if (k == 5)
					boxPos.z += boxSize.z;
				if (k != 2 && k != 3)
				{

					exclusion = Ray::CheckRayPlane(m_Normal[k], boxPos, m_Position, TargetDir, Dis);
					if (exclusion.hit)
					{
						vtx = box->GetComponent<Collision>()->GetVertices();
						switch (k)
						{
						case 0:
							triangle[0] = { vtx.pos[0],vtx.pos[2],vtx.pos[4] };
							triangle[1] = { vtx.pos[2],vtx.pos[6],vtx.pos[4] };
							break;
						case 1:
							triangle[0] = { vtx.pos[3],vtx.pos[1],vtx.pos[7] };
							triangle[1] = { vtx.pos[1],vtx.pos[5],vtx.pos[7] };
							break;
						case 4:
							triangle[0] = { vtx.pos[2],vtx.pos[3],vtx.pos[6] };
							triangle[1] = { vtx.pos[3],vtx.pos[7],vtx.pos[6] };
							break;
						case 5:
							triangle[0] = { vtx.pos[1],vtx.pos[0],vtx.pos[5] };
							triangle[1] = { vtx.pos[0],vtx.pos[4],vtx.pos[5] };
							break;
						default:
							break;
						}

						point = exclusion.point;
						for (int j = 0; j < 2; j++)
						{
							result = Ray::CheckPointTraiangle(point, triangle[j]);
							//��Q�����Ԃɂ����true
							if (result.hit)
							{
								checkFlg = true;
								//�ǂ̕����Ȃ̂�����
								switch (i)
								{
								case 0:
									m_DirObj[0] = true;
									break;
								case 1:
									m_DirObj[1] = true;
									break;
								case 2:
									m_DirObj[2] = true;
									break;
								case 3:
									m_DirObj[3] = true;
									break;
								default:
									break;
								}
								break;
							}
							//�ǂ̕����Ȃ̂�����
							switch (i)
							{
							case 0:
								m_DirObj[0] = false;
								break;
							case 1:
								m_DirObj[1] = false;
								break;
							case 2:
								m_DirObj[2] = false;
								break;
							case 3:
								m_DirObj[3] = false;
								break;
							default:
								break;

							}
						}

					}
				}
				if (checkFlg)
					break;
			}
			if (checkFlg)
				break;
		}

	}
}
