#include "tankAvoid.h"
#include "box.h"
#include "ray.h"
#include "collision.h"
#include "pbullet.h"
#include "CreateLine.h"
#include "manager.h"
#include "XMFLOAT_Helper.h"
//=======================
//		������
//=======================
void TankAvoid::Init()
{
	scene = Manager::GetScene();	//�V�[��
	m_avertDelay = 2.0f;
	m_avertCount = 10.0f;
	m_MoveTime = 3.0f;
	m_HitBullet.hit = false;
	m_AvoidFlg = false;
	
}
//=======================
//		������
//=======================
bool TankAvoid::Avoid()
{
	m_Position = m_GameObject->GetPosition();//���W
	m_Rotation = m_GameObject->GetRotation();//�p�x
	m_Forward = m_GameObject->GetForward();	 //�O���x�N�g��
	m_Speed = m_GameObject->GetSpeed();		 //���x
	m_CollisionFlg = m_GameObject->GetCollisionFlg();	//�����蔻��t���O
	bool stateFlg = false;

	//�e�������肻��������
	Possibility();
	//true�̎������Ȃ�
	if (!m_AvoidFlg)
	{
		//�����肻���ȂƂ����ɓ���
		if (m_HitBullet.hit)
		{
			//4����
			for (int i = 0; i < 4; i++)
			{
				//���������
				//�O�x�A�X�O�x�A�|�X�O�x�A�P�W�O�x
				switch (i)
				{
				case 0:
					m_RayDir[i] = m_Forward;
					break;
				case 1:
					m_RayDir[i] = { m_Forward.z,0.0f,-m_Forward.x };
					break;
				case 2:
					m_RayDir[i] = { -m_Forward.z,0.0f,m_Forward.x };
					break;
				case 3:
					m_RayDir[i] = { -m_Forward.x,0.0f,-m_Forward.z };
					break;

				default:
					break;
				}
			}
			//box�̏����擾
			std::vector<Box*> boxList = scene->GetGameObjects<Box>();
			//box�̐���
			for (Box* box : boxList)
			{
				bool Endflg = false;//true�ɂȂ������������ʂ���
				//�@���̐�������
				for (int i = 0; i < 4; i++)
				{
					//���C���΂������̐�������
					for (int j = 0; j < 4; j++)
					{
						DirectX::XMFLOAT3 boxPos = box->GetPosition();//���W
						DirectX::XMFLOAT3 boxSize = box->GetScale();  //�T�C�Y
						//�@���̕����̃T�C�Y��������
						switch (i)
						{
						case 0:
							boxPos.x -= boxSize.x;
							break;
						case 1:
							boxPos.x += boxSize.x;
							break;
						case 2:
							boxPos.z -= boxSize.z;
							break;
						case 3:
							boxPos.z += boxSize.z;
							break;
						default:
							break;
						}

						//�㉺���E�Ƀ��C���΂�
						Ray::Result result = Ray::CheckRayPlane(m_Dir4[i],
																boxPos, 
																m_Position, 
																m_RayDir[j], 
																30.0f);
						//Ray��box�ɓ������������ɓ���
						if (result.hit)
						{
							//���C�������������W���i�[
							m_FacePoint[j] = result.point;
							Endflg = true;	//�������ʂ���
							if (Endflg)
								break;
						}
						if (Endflg)
							break;
					}
					if (Endflg)
						break;
				}
				if (Endflg)
					continue;
			}
			//���C�����������ʂ܂ł̋������v�Z���A�\�[�g
			LenSort();
			m_AvoidFlg = true;
			//���ς̔���ō��E
			m_tempAngle = PointAngle();
			m_PAngle = 0.0f;		//������
			m_MoveCount = 0.0f;		//������
			DebugOneFlg = DebugFlg;	//������
		}
	}
	//Count�̕����傫����Β��ɓ���
	if (m_avertCount >= m_avertDelay)
	{
		//�f�o�b�O�p��line
		CreateLine* line;
		line = scene->GetGameObject<CreateLine>();

		//�p�x��␳
		if (m_PAngle > 6.28f)
			m_PAngle -= 6.28f;
		if (m_PAngle < 0.0f)
			m_PAngle += 6.28f;

		//����������Ɍ����Ă���Β��ɓ���
		if (m_PAngle <= m_tempAngle + 0.05f && m_PAngle >= m_tempAngle - 0.05f)
		{
			//�ړ�����v�Z�i�f�o�b�N�\���p
			if (DebugFlg)
			{
				DebugPos = m_Position;
				DebugTime = m_MoveCount;
				while (DebugOneFlg)
				{
					DebugPos.x += m_Forward.x * m_Speed.x;
					DebugPos.z += m_Forward.z * m_Speed.z;

					DebugTime += 1.0f / 60.0f;
					if (DebugTime >= m_MoveTime)
					{
						line = scene->AddGameObject<CreateLine>();
						line->PosCreate(DebugPos, m_Position);
						DebugOneFlg = false;
					}
				}
				line->PosCreate(DebugPos, m_Position);
			}
			//�ړ�
			m_Position.x += m_Forward.x * m_Speed.x;
			m_Position.z += m_Forward.z * m_Speed.z;
		}
		else
		{
			//����������ɉ�]
			if (m_tempAngle <= 3.14)
			{
				m_Rotation.y += 0.05f;
				m_PAngle += 0.05f;
			}
			else
			{
				m_Rotation.y -= 0.05f;
				m_PAngle -= 0.05f;
			}
		}
		//��莞�ԓ�����
		if (m_MoveCount >= m_MoveTime || m_CollisionFlg){
			//������
			m_AvoidFlg = false;
			m_avertCount = 0.0f;
			stateFlg = true;
			//�f�o�b�O�pline���폜
			if (line != nullptr)
				line->SetDestroy();
		}
		//�����鎞�Ԃ̃J�E���g
		m_MoveCount += 1.0f / 60.0f;

	}
	else if (m_AvoidFlg &&m_avertCount <= m_avertDelay)
		stateFlg = true;	//�X�e�[�g��J��
	//����n��
	m_GameObject->SetPosition(m_Position);
	m_GameObject->SetRotation(m_Rotation);

	return stateFlg;
}
//=======================
//			�����\�[�g
//=======================
void TankAvoid::LenSort()
{
	//������
	DirectX::XMFLOAT3 point[4] = { {0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	
	for (int i = 0; i < 4; i++)
	{
		//�|�C���g�܂ł̃x�N�g��
		point[i].x = m_Position.x - m_FacePoint[i].x;
		point[i].z = m_Position.z - m_FacePoint[i].z;

		//��Βl�ɕϊ�
		point[i].x = fabsf(point[i].x);
		point[i].z = fabsf(point[i].z);
		//�ړ��x�N�g���̌v�Z
		m_Vec[i] = sqrt(point[i].x * point[i].x +
			point[i].z * point[i].z);
	}
	//�\�[�g
	for (int i = 0; i < 4; i++) {
		for (int j = i; j < 4; j++) {
			if (m_Vec[i] > m_Vec[j]) {
				//�l�̌���
				float temp = m_Vec[i];
				m_Vec[i] = m_Vec[j];
				m_Vec[j] = temp;

				//�����������W���\�[�g
				DirectX::XMFLOAT3 temp3 = m_FacePoint[i];
				m_FacePoint[i] = m_FacePoint[j];
				m_FacePoint[j] = temp3;

			}
		}
	}
}
//================================
//		�O�ς��g�����p�x�̌v�Z
//================================
float TankAvoid::PointAngle()
{
	//�ϐ���`
	float Rot;
	DirectX::XMFLOAT3 tankDir;
	DirectX::XMFLOAT3 RotateDir;

	//���K��
	DirectX::XMVECTOR DirVector = DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&m_FacePoint[3]));

	DirectX::XMStoreFloat3(&tankDir, DirVector);
	//���ς̌v�Z
	DirectX::XMStoreFloat(&Rot, DirectX::XMVector3Dot(
		DirectX::XMVector3Normalize(DirVector),
		DirectX::XMLoadFloat3(&m_Forward)));
	Rot = ::acos(Rot);
	//�O�ς̌v�Z
	DirectX::XMStoreFloat3(&RotateDir,
		DirectX::XMVector3Cross(
			DirectX::XMVector3Normalize(DirVector),
			DirectX::XMLoadFloat3(&m_Forward)));
	//�p�x�̕␳
	if (RotateDir.y > 0) {
		Rot = 6.28 - Rot;
	}
	return Rot;
}
//===================================
//		�G���e�����F�ł��Ă��邩����
//===================================
void TankAvoid::Possibility()
{
	//����������O�ɏ�����
	m_HitBullet.dot = 0.0f;
	m_HitBullet.hit = false;
	m_HitBullet.pos = { 0.0f,0.0f,0.0f };
	m_HitBullet.velo = { 0.0f,0.0f,0.0f };
	//�ϐ���`
	Ray::Result result;
	Ray::Result exclusion;
	Ray::Triangle triangle[2];
	Collision::Vertices vtx;
	DirectX::XMFLOAT3 point;
	float rotY = 0.0f;
	bool checkFlg = false;
	//�e�̏����V�[��������擾
	pbullet = scene->GetGameObjects<PBullet>();
	//�V�[�����ɂ���e�̐���
	for (PBullet* bullet : pbullet)
	{
		DirectX::XMFLOAT3 bulletVec = bullet->GetVelocity();	//�e�̃x�N�g��
		DirectX::XMFLOAT3 bulletPos = bullet->GetPosition();	//�e�̍��W
		DirectX::XMFLOAT3 TargetDir = 							//�e�ƃ^���N�̋���
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
						//���C�̌v�Z
						exclusion = Ray::CheckRayPlane(m_Normal[k], boxPos, m_Position, TargetDir, Dis + 0.1f);
						if (exclusion.hit)
						{
							//���_�����擾
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
			//�e�������肻���Ȃ璆�ɓ���
			if (!checkFlg)
			{
				//�e�ƃ^���N�̊p�x
				rotY = face(bulletPos);
				//�e�����m����p�x
				if (rotY <= 0.3f && rotY >= -0.3f)
				{
					//�����肻�������ς��g���Čv�Z
					m_HitBullet = Dot(bulletVec, bulletPos);
					break;
				}
			}
		}
	}
}
//===================================
//		���ςŊp�x�����߂�
//===================================
float TankAvoid::face(DirectX::XMFLOAT3 targetPos)
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
//===================================
//		���ς̌��ʂ�HitBullet�Ɋi�[
//===================================
TankAvoid::HitBullet TankAvoid::Dot(DirectX::XMFLOAT3 vec, DirectX::XMFLOAT3 pos)
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
		hit.pos = { 0.0f,0.0f,0.0f };
		hit.velo = { 0.0f,0.0f,0.0f };
		hit.dot = 0.0f;
	}
	return hit;
}
