// Tank�I�u�W�F�N�g [tank.cpp]
#include "tank.h"
#include <math.h>
#include "shader.h"
#include "modelRenderer.h"
#include "bullet.h"
#include "box.h"
#include "input.h"
#include "ray.h"
#include "collision.h"
#include "CreateLine.h"
#include "alongthewall.h"
#include "tankMove.h"
#include "tankShot.h"
#include "tankAvoid.h"

//time
#include <time.h>

#include "XMFLOAT_Helper.h"

//ImGui
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"
//===================================
//				������
//===================================
void Tank::Init()
{
	//�R���|�[�l���g�̒ǉ�
	AddComponent<Shader>()->Load("shader/vertexLightingVS.cso",			//�V�F�[�_
		"shader/vertexLightingPS.cso");
	AddComponent<ModelRenderer>()->Load("asset/model/ETank.obj");		//���f��					
	AddComponent<AlongTheWall>()->SetTank(this);						//�����蔻��
	tankMove = AddComponent<TankMove>();								//�ړ�
	tankShot = AddComponent<TankShot>();								//�e������
	tankAvoid = AddComponent<TankAvoid>();								//������
	astar = AddComponent<AStarMove>();									//AStar
	//������
	m_avertDelay = 3.0f;	//�ēx���Ă�悤�ɂȂ�܂ł̎���
	m_avertCount = 3.0f;	//�n�܂�������ł��e�����悤�ɒ���
	m_hitbullet.hit = false;
	
}
//===================================
//				�X�V
//===================================
void Tank::Update()
{
	//�v���C���[�����Ȃ���Έȍ~�̏������s��Ȃ�
	if (player == NULL)
		return;
	//�J�E���g
	m_avertCount += 1.0f / 60.0f;
	//�O�����x�N�g���̎擾
	m_Forward = GetForward();
	//Rotation�̕␳
	if (m_Rotation.y > 6.28f)
		m_Rotation.y -= 6.28f;
	if (m_Rotation.y < 0.0f)
		m_Rotation.y += 6.28f;
	//�X�e�[�g�̑J��
	switch (m_currentState)
	{
	case Tank::donot:	//�ҋ@
		DoNot();
		break;
	case Tank::move:	//�ړ�
		Move();
		break;
	case Tank::shot:	//�ˌ�
		Shot();
		break;
	case Tank::avoid:	//������
		Avoid();
		break;
	default:
		break;
	}

	//-----ImGui-----
	if (DebugFlg)	//�I���̎��̂ݕ\��
	{
		ImGui::Begin("Tank");
		ImGui::Text("Pos: x:%.2f,y:%.2f,z:%.2f", m_Position.x, m_Position.y, m_Position.z);
		ImGui::Text("Rotation: x:%.2f,y:%.2f,z:%.2f", m_Rotation.x, m_Rotation.y, m_Rotation.z);
		ImGui::Text("RotY: %.2f", m_RotY);
		ImGui::Text("State: %s", statenamu[m_currentState]);
		ImGui::End();
	}
}
//===================================
//				�ҋ@
//===================================
void Tank::DoNot()
{
	//�^���N�ƃv���C���[�̊ԂɃI�u�W�F�N�g�����邩����
	Vision();
	
	//�ԂɃI�u�W�F�N�g���Ȃ���Βe������
	if (!m_excFlg)
	{
		if (m_avertCount >= m_avertDelay)
		{
			//�e�������肻��������
			Possibility();
			{
				if (m_hitbullet.hit)
				{
					ChangeState(Tank::avoid);	//������
					return;
				}
			}
		}
		ChangeState(Tank::shot);			//�e������
	}
	else
	{
		ChangeState(Tank::move);			//�ړ�
	}

}
//===================================
//				�ړ�
//===================================
void Tank::Move()
{
	bool moveflg = false;

	moveflg = astar->Navi();		//�t���O���擾
	//true�̎��J��
	if (moveflg)					
		ChangeState(Tank::donot);	//�ҋ@�ɑJ��
}
//===================================
//				�e������
//===================================
void Tank::Shot()
{
	bool shotFlg = false;
	shotFlg = tankShot->Shot();		//�t���O���擾
	//true�̎��J��
	if(shotFlg)
		ChangeState(Tank::donot);	//�ҋ@�ɑJ��
}
//===================================
//				������
//===================================
void Tank::Avoid()
{
	bool avoidFlg = false;
	avoidFlg = tankAvoid->Avoid();		//�t���O���擾
	//true�̎��J��
	if (avoidFlg)
		ChangeState(Tank::donot);		//�ҋ@�ɑJ��
}
//===================================
//		���ςŊp�x�����߂�
//===================================
float Tank::face(DirectX::XMFLOAT3 targetPos)
{
	//=====�ϐ���`=====
	DirectX::XMFLOAT3 TargetDir;	//�ړI�n�܂ł̋���
	DirectX::XMVECTOR DirVector;	//���K����������
	float rotY = 0.0f;				//�p�x
	DirectX::XMFLOAT3 forward = GetForward();	//�O���x�N�g��
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
//==================================================================
//			�^���N�ƃv���C���[�̊ԂɃI�u�W�F�N�g�����邩����
//==================================================================
void Tank::Vision()
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
//===================================
//		�G���e�����F�ł��Ă��邩����
//===================================
void Tank::Possibility()
{
	//����������O�ɏ�����
	m_hitbullet.dot = 0.0f;
	m_hitbullet.hit = false;
	m_hitbullet.pos = { 0.0f,0.0f,0.0f };
	m_hitbullet.velo = {0.0f,0.0f,0.0f};
	//�ϐ���`
	Ray::Result result;			
	Ray::Result exclusion;		
	Ray::Triangle triangle[2];
	Collision::Vertices vtx;
	DirectX::XMFLOAT3 point;
	std::vector<PBullet*> pbullet;	//�e�̏��
	float rotY = 0.0f;

	bool checkFlg = false;

	//�e�̏����V�[��������擾
	pbullet = scene->GetGameObjects<PBullet>();
	//�V�[�����ɂ���e�̐��񂷁B
	for (PBullet* bullet : pbullet)
	{
		DirectX::XMFLOAT3 bulletVec = bullet->GetVelocity();	//�e�̃x�N�g��
		DirectX::XMFLOAT3 bulletPos = bullet->GetPosition();	//�e�̍��W
		DirectX::XMFLOAT3 TargetDir =							//�e�ƃ^���N�̋���
			DirectX::XMFLOAT3(bulletPos.x - m_Position.x, 
							  bulletPos.y - m_Position.y, 
							  bulletPos.z - m_Position.z);
		//�x�N�g���̌v�Z
		float Dis = sqrtf(TargetDir.x * TargetDir.x + TargetDir.z * TargetDir.z);
		//�V�[������box���擾
		std::vector<Box*> boxList = scene->GetGameObjects<Box>();
		//��苗�����߂����̂ݏ���
		if (Dis <= 15.0f)
		{
			//box�̐��񂷁B
			for (Box* box : boxList)
			{
				//�@���̐�������
				for (int k = 0; k < 6; k++)
				{

					DirectX::XMFLOAT3 boxPos = box->GetPosition();	//���W
					DirectX::XMFLOAT3 boxSize = box->GetScale();	//�T�C�Y

					if (k == 0)
						boxPos.x -= boxSize.x;
					if (k == 1)
						boxPos.x += boxSize.x;
					if (k == 4)
						boxPos.z -= boxSize.z;
					if (k == 5)
						boxPos.z += boxSize.z;
					//Y�������O
					if (k != 2 && k != 3)
					{
						exclusion = Ray::CheckRayPlane(m_Normal[k], boxPos, m_Position, TargetDir, Dis + 0.1f);
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
									break;
								}
							}
							if (checkFlg)
								break;
						}
					}
				}
				if (checkFlg)
					break;
			}
			//�e�ƃ^���N�̊ԂɃI�u�W�F�N�g������ ���X�e�[�g��avoid�̏ꍇ
			if (!checkFlg && Tank::avoid)
			{
				//�e�ƃ^���N�̊p�x
				rotY = face(bulletPos);
				//�e�����m����͈�
				if (rotY <= 0.3f && rotY >= -0.3f)
				{
					//�����肻�������ς��g���Čv�Z
					m_hitbullet = Dot(bulletVec,bulletPos);
					//�f�o�b�O���I���̎��Ɏ��s
					if (DebugFlg)
					{
						//========�e�̐F��ς���==========
						ChangeColor(bullet);
						//===============================
					}
					
					break;
				}
			}
		}
	}
}
//===================================
//		���ς̌��ʂ�HitBullet�Ɋi�[
//===================================
Tank::HitBullet Tank::Dot(DirectX::XMFLOAT3 vec,DirectX::XMFLOAT3 pos)
{
	//�\���̐錾
	HitBullet hit;					
	//�ϐ���`
	DirectX::XMFLOAT3 bulletVelo;	//�e�̑��x
	DirectX::XMFLOAT3 bulletdis;	//����
	float dot;						//�v�Z����
	//���K��
	DirectX::XMStoreFloat3(&bulletVelo, DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&vec)));

	//�e�܂ł̋���
	bulletdis = pos - m_Position;
	//���K��
	DirectX::XMStoreFloat3(&bulletdis, DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&bulletdis)));
	//���ς̌v�Z
	DirectX::XMStoreFloat(&dot, DirectX::XMVector3Dot(
		DirectX::XMLoadFloat3(&bulletVelo),
		DirectX::XMLoadFloat3(&bulletdis)));
	//�ēx�͈͓�������
	if (dot <= -0.96)
	{
		//�v�Z���ʂ��i�[
		hit.hit = true;
		hit.pos = pos;
		hit.velo = bulletVelo;
		hit.dot = dot;
	}
	else
	{
		//�͈͊O�̎��ɏ�����
		hit.hit = false;
		hit.pos = {0.0f,0.0f,0.0f};
		hit.velo = { 0.0f,0.0f,0.0f };
		hit.dot = 0.0f;
	}
	return hit;
}

//===========================================
//			�e�̐F��ς���
//===========================================
void Tank::ChangeColor(PBullet * bullet)
{
	//�e�̐F��ς���
	bullet->ChangeColor();
}

