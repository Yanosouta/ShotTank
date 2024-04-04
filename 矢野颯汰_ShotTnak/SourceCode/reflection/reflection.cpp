#include "reflection.h"
#include "manager.h"
//==========================
//			������
//==========================
void Reflection::Init()
{
	//�V�[���̏����擾
	m_Scene = Manager::GetScene();
	//�V�[�����̃u���b�N�̏����擾
	m_Box = m_Scene->GetGameObjects<Box>();
	//���˂̉�
	m_Refl = 1;
	//���˂̐�����
	m_ReflCount = 0;
}
//===========================
//			�X�V
//===========================
void Reflection::Update()
{
	//�e���Ȃ���Έȍ~�̏������s��Ȃ�
	if (m_Bullet == NULL)
		return;
	//�e�̏����擾
	m_BulletPos = m_GameObject->GetPosition();
	m_BulletScale = m_GameObject->GetScale();
	m_BulletVel = m_GameObject->GetVelocity();
	//box�̐�������
	for (int i = 0; i < m_Box.size(); i++)
	{
		//�u���b�N���Ƃ̏����擾
		m_BoxPos = m_Box[i]->GetPosition();
		m_BoxScale = m_Box[i]->GetScale();

		//�����蔻��
		if (UpCollision(m_BoxPos, m_BoxScale) &&
			DownCollision(m_BoxPos, m_BoxScale) &&
			LeftCollision(m_BoxPos, m_BoxScale) &&
			RightCollision(m_BoxPos, m_BoxScale))
		{
			//�ǂ̖ʂɓ�������������
			if (m_BulletPos.x < m_BoxPos.x  && m_BulletPos.z >= (m_BoxPos.z - m_BoxScale.z) && m_BulletPos.z <= (m_BoxPos.z + m_BoxScale.z))//��
				m_TouchNor = m_Normal[1];
			else if (m_BulletPos.x > m_BoxPos.x  && m_BulletPos.z >= (m_BoxPos.z - m_BoxScale.z) && m_BulletPos.z <= (m_BoxPos.z + m_BoxScale.z))//��
				m_TouchNor = m_Normal[0];
			else if (m_BulletPos.z < m_BoxPos.z  && m_BulletPos.x >= (m_BoxPos.x - m_BoxScale.x) && m_BulletPos.x <= (m_BoxPos.x + m_BoxScale.x))//��
				m_TouchNor = m_Normal[4];
			else if (m_BulletPos.z > m_BoxPos.z  && m_BulletPos.x >= (m_BoxPos.x - m_BoxScale.x) && m_BulletPos.x <= (m_BoxPos.x + m_BoxScale.x))//�E
				m_TouchNor = m_Normal[5];

			//���˃x�N�g���̌v�Z
			m_BulletVel.x = m_BulletVel.x + 2.0f*(-m_BulletVel.x * m_TouchNor.x) * m_TouchNor.x;
			m_BulletVel.z = m_BulletVel.z + 2.0f*(-m_BulletVel.z * m_TouchNor.z) * m_TouchNor.z;
			//�l��Ԃ�
			m_GameObject->SetVelocity(m_BulletVel);
		
			m_ReflCount++;
			
		}
	}
	//�K��ȏ�̉񐔔��˂�����폜
	if(m_ReflCount > m_Refl)
		m_GameObject->SetDestroy();
}
//==============================
//		��̓����蔻��
//==============================
bool Reflection::UpCollision(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 scale)
{
	return pos.x - scale.x - 0.5f < m_BulletPos.x;
}
//==============================
//		���̓����蔻��
//==============================
bool Reflection::DownCollision(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 scale)
{
	return m_BulletPos.x < pos.x + scale.x + 0.5f;
}
//==============================
//		���̓����蔻��
//==============================
bool Reflection::LeftCollision(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 scale)
{
	return pos.z - scale.z - 0.5f < m_BulletPos.z;
}
//==============================
//		�E�̓����蔻��
//==============================
bool Reflection::RightCollision(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 scale)
{
	return m_BulletPos.z < pos.z + scale.z + 0.5f;
}
//==============================
//		�G�̒e�̏����擾
//==============================
void Reflection::SetBullet(Bullet* bullet)
{
	m_Bullet = bullet;
}
//==============================
//		�G�̒e�̏����擾
//==============================
void Reflection::SetPBullet(PBullet* pbullet)
{
	m_PBullet = pbullet;
}
