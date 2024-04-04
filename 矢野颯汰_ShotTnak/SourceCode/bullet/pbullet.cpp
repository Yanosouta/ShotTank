// Bullet�I�u�W�F�N�g [bullet.cpp]
#include "pbullet.h"
#include "modelRenderer.h"
#include "shader.h"
#include "explosion.h"
#include "player.h"
#include "reflection.h"
#include "tank.h"
using DirectX::operator*;

//=====================
//		������
//=====================
void PBullet::Init()
{
	//�R���|�[�l���g�̒ǉ�
	AddComponent<Shader>()->Load(										//�V�F�[�_
		"shader/vertexLightingVS.cso", "shader/vertexLightingPS.cso");
	AddComponent<ModelRenderer>()->Load("asset/model/Pbullet.obj");		//���f��
	AddComponent<Reflection>()->SetPBullet(this);						//����
}
//=====================
//		�@�X�V
//=====================
void PBullet::Update()
{
	//�V�[���̏����擾
	scene = Manager::GetScene();
	//�^���N�̏����擾
	std::vector<Tank*> tankList = scene->GetGameObjects<Tank>();

	//�e�̈ړ�
	m_Position.x += m_Velocity.x;
	m_Position.y += m_Velocity.y;
	m_Position.z += m_Velocity.z;
	//�^���N�����Ȃ���Έȍ~�̏������s��Ȃ�
	if (tankList.size() == NULL)
		return;

	//�����蔻��̌v�Z
	//�^���N�̐���
	for (Tank* tank : tankList)
	{
		//���W�̎擾
		DirectX::XMFLOAT3 position = tank->GetPosition();
		//�x�N�g��
		DirectX::XMFLOAT3 direction;
		//�x�N�g���̌v�Z
		direction.x = position.x - m_Position.x;
		direction.y = position.y - m_Position.y;
		direction.z = position.z - m_Position.z;
		//�����̌v�Z
		DirectX::XMStoreFloat(&m_length,
			DirectX::XMVector3LengthSq(DirectX::XMLoadFloat3(&direction)));
		//�������Ă��邩����
		if (m_length < (2.0f * 2.0f))
		{
			//�G�t�F�N�g
			Explosion* explosion = scene->AddGameObject<Explosion>();
			explosion->SetScale(DirectX::XMFLOAT3(1.5f, 1.5f, 1.0f));//�T�C�Y
			explosion->SetPosition(DirectX::XMFLOAT3(position.x,	 //���W
				position.y + 1.0f, position.z));

			//�폜
			tank->SetDestroy();	//�^���N�̍폜
			SetDestroy();		//�e�̍폜
			return;
		}
	}
	
}

//=============================
//	  �e�̐F�̕ύX(�f�o�b�O�p)
//=============================
void PBullet::ChangeColor()
{
	GetComponent<ModelRenderer>()->Load("asset/model/Debugbullet.obj");
}

