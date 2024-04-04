#include "AStarMove.h"
#include "ray.h"
#include "collision.h"
#include "box.h"
#include "gameObject.h"
#include "scene.h"
#include "player.h"
#include <list>
#include <thread>
#include <future>

//================================
//				������
//================================
void AStarMove::Init()
{
	//AStar
	astar = m_GameObject->AddComponent<AStar>();

	//�ϐ���`
	float x, y ,num;
	num = MAP_HEIGHT;
	int count = 0;
	for (int i = 0; i < MAP_HEIGHT; i++)
	{
		x = num * 2 ;
		for (int j = 0; j < MAP_WIDTH; j++)
		{
			y = j * 2 + 2;
			m_MapPos[i][j] = { x,y };	//�ڕW�n�_�̍��W���i�[
			//�f�o�b�O���I���̎��ɕ\��
			if (m_DebugFlg) {
				m_CBall[i][j] = scene->GetGameObject<CreateBall>();	//�����擾
				m_CBall[i][j] = scene->AddGameObject<CreateBall>();	//�\��
				m_CBall[i][j]->SetPosition(DirectX::XMFLOAT3(m_MapPos[i][j].y, 1.0f, m_MapPos[i][j].x));	//���W���Z�b�g
				m_CBall[i][j]->SetScale(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));								//�T�C�Y���Z�b�g
			}	
			count++;
		}
		num--;
	}


}
//=========================================
//		�v���C���[�Ɍ������Ĉړ�
//=========================================
bool AStarMove::Navi()
{
	//�v���C���[�̏����擾
	player = scene->GetGameObject<Player>();
	//�v���C���[�����Ȃ��Ƃ��͈ȍ~�̏������s��Ȃ��B
	if (player == NULL)
		return false;
	//�v���C���[�̍��W���擾
	DirectX::XMFLOAT3 playerPos = player->GetPosition();

	//�e�����擾
	m_Position = m_GameObject->GetPosition();	//���W
	m_Rotation = m_GameObject->GetRotation();	//�p�x
	m_Forward = m_GameObject->GetForward();		//�O���x�N�g��

	//�ϐ���`
	DirectX::XMINT2 index;	//�z��̐��l
	float dist;				//����
	
	bool flg = false;		//2��ڈȍ~����t���O

	//��ԋ߂��|�C���g���i�[
	DirectX::XMINT2 startIndex = NearPoint(m_Position);
	//�ړI�n�_(�v���C���[�̍��W)���i�[
	DirectX::XMINT2 EndIndex = NearPoint(playerPos);
	
	//�����ݒ�܂��͕ʂ̃X�e�[�g����J�ڂ�������Ɏ��s
	if (m_InitOneFlg || m_StateFlg)
	{
		//AStar�}�b�v�̐���
		astar->CreateMap();
		//�ړI�n�܂ł̃m�[�h�����߂�
		astar->AStarSearch(startIndex, EndIndex);
		//�ړI�n�܂ł̃m�[�h���i�[
		m_astarList = astar->GetAStar(m_astarList);
		m_InitOneFlg = false;
		m_StateFlg = false;
	}
	else {
		flg = true;
	}

	//��x���s�����玟�̃|�C���g�ɒ����܂Ŏ��s���Ȃ�
	if (m_AStarFlg)//�|�C���g�ɓ���
	{
		//AStar�}�b�v�̐���
		//astar->CreateMap();

		//�m�[�h���擾
		if(flg)
			m_astarList = astar->GetAStar(m_astarList);

		//�m�[�h�̈�Ԗڂ��擾
		auto it = m_astarList.begin();
		//�m�[�h�����鎞true
		if (m_astarList.size() > 0)
		{
			//���X�g������炷(�ڕW�n�_)
			index = *++it;
			//�ڕW�n�_�̍��W���i�[
			m_PointPos = DirectX::XMFLOAT3(m_MapPos[index.y][index.x].y,
				0.0f,
				m_MapPos[index.y][index.x].x);
		}
		
		//�o�H�̉���
		if (m_DebugFlg) {
			//�O��̌o�H�̐F��߂�
			for (int i = 0; i < MAP_HEIGHT; i++)
			{
				for (int j = 0; j < MAP_WIDTH; j++)
				{
					ReturnColor(m_CBall[i][j]);
				}
			}
			//�o�H�̐F��ς���
			auto itr = m_astarList.begin();
			index = *itr;
			//�ړI�n�܂ł�Ball�̐F��ύX
			for (int i = 0; i < m_astarList.size() - 1; i++)
			{
				ChangeColor(m_CBall[index.y][index.x]);
				index = *++itr;
			}
		}
		m_AStarFlg = false;
		m_MoveOneFlg = true;
	}
	else
	{
		//�ړ����Ɉ�x�����v�Z���s��
		if (m_MoveOneFlg)
		{
			//��ԋ߂��|�C���g���i�[
			DirectX::XMINT2 startIndex = NearPoint(m_PointPos);
			DirectX::XMINT2 EndIndex = NearPoint(playerPos);
			//�ړI�n�܂ł̃m�[�h�����߂�
			astar->AStarSearch(startIndex, EndIndex);
		}
		m_MoveOneFlg = false;
	}
	//�ړI�n�܂ł̊p�x���v�Z
	m_RotY = face(m_PointPos);
	//�x�ɕϊ�
	m_Degrees = ToDegrees(m_RotY);
	//�ړI�n�ɓ�����Ɉ�x�␳
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
	//��ԋ߂��|�C���g�܂ł̋������X�V
	dist = Dist(m_PointPos, m_Position);

	//�ړI�n�̕����ɉ�]
	if (m_Degrees >= 2.5f || m_Degrees <= -2.5f)
	{
		if (m_Degrees >= 0.0f) {
			m_Rotation.y += ToRadian(1.5f);		//�E���
		}
		else {
			m_Rotation.y -= ToRadian(1.5f);		//�����
		}
	}
	else {//��]����K�v���Ȃ���Έړ�
		if (m_PointPos.x > 0.1f)
			m_Position.x += m_Forward.x * m_Speed.x;
		if (m_PointPos.z > 0.1f)
			m_Position.z += m_Forward.z * m_Speed.z;
	}
	//�ړI�n�ɒ�������t���O�����Z�b�g
	if (dist < 0.2f)
	{
		m_OneFlg = true;
		m_AStarFlg = true;
	}

	//�^���N�ƃv���C���[�̊ԂɃI�u�W�F�N�g�����邩����
	Vision();		
	//�ԂɃI�u�W�F�N�g���Ȃ���Ύ��s
	if (!m_excFlg)
	{
		m_correctedShotTime += 1.0f / 60.0f;
		//�v���C���[�������Ē����ɒe�������Ȃ��悤�Ɏ��ԂŒ���
		if (m_correctedShotTime >= 0.5f)
		{
			//������
			m_correctedShotTime = 0.0f;
			m_InitOneFlg = true;
			m_StateFlg = true;
			m_AStarFlg = true;
		}

	}
	//�^���N�̏����Z�b�g
	m_GameObject->SetPosition(m_Position);	//���W
	m_GameObject->SetRotation(m_Rotation);	//�p�x

	return m_StateFlg;
}
//===================================
//		���ςŊp�x�����߂�
//===================================
float AStarMove::face(DirectX::XMFLOAT3 targetPos)
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
//==========================================
//��ԋ߂��|�C���g�����߂�
//==========================================
DirectX::XMINT2 AStarMove::NearPoint(DirectX::XMFLOAT3 pos)
{
	//�ϐ���`
	float map[MAP_HEIGHT][MAP_WIDTH];	//�}�b�v
	float temp = 5.0f;					//�����̏����l
	int num = MAP_HEIGHT;				//�c
	//�z��Ɋi�[���ꂽ���W�܂ł̋��������߂�
	for (int i = 0; i < MAP_HEIGHT; i++)
	{
		num--;
		for (int j = 0; j < MAP_WIDTH; j++)
		{
			//���������߂�
			map[num][j] = Dist(DirectX::XMFLOAT3(m_MapPos[num][j].y, 0.0f, m_MapPos[num][j].x), pos);
			//���߂鐔�l�̕�����������Ύ��s
			if (map[num][j] < temp)
			{
				temp= map[num][j];			//�u��������
				m_MapIndex = { j,num };
			}
		}
	}
	//��ԋ߂��|�C���g��Ԃ�
	return m_MapIndex;
}
//==================================================================
//			�^���N�ƃv���C���[�̊ԂɃI�u�W�F�N�g�����邩����
//==================================================================
void AStarMove::Vision()
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
