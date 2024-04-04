// Bullet�I�u�W�F�N�g [bullet.cpp]
#include "bullet.h"
#include "modelRenderer.h"
#include "shader.h"
#include "tank.h"
#include "explosion.h"
#include "reflection.h"

using DirectX::operator*;
//=========================
//			������
//=========================
void Bullet::Init()
{
	//�R���|�[�l���g�̒ǉ�
	AddComponent<Shader>()->Load(											//�V�F�[�_
		"shader/vertexLightingVS.cso", "shader/vertexLightingPS.cso");		
	AddComponent<ModelRenderer>()->Load("asset/model/Debugbullet.obj");		//���f��
	AddComponent<Reflection>()->SetBullet(this);							//����
}
//=========================
//			�X�V
//=========================
void Bullet::Update()
{
	//�V�[���̏����擾
	scene = Manager::GetScene();
	//�v���C���[�̏����擾
	player = scene->GetGameObject<Player>();
	//�v���C���[�����Ȃ���Έȍ~�̏������s��Ȃ�
	if (player == NULL)
		return;
	//�e�̈ړ�
	m_Position.x += m_Velocity.x;
	m_Position.y += m_Velocity.y;
	m_Position.z += m_Velocity.z;
	
	//�����蔻��
	collision();
}
//=========================
//		�����蔻��
//=========================
void Bullet::collision()
{	
	//���W�̎擾
	DirectX::XMFLOAT3 position = player->GetPosition();
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
		
		explosion->SetScale(DirectX::XMFLOAT3(1.5f, 1.5f, 1.0f));	//�T�C�Y���Z�b�g
		explosion->SetPosition(DirectX::XMFLOAT3(position.x,		//���W���Z�b�g
			position.y + 1.0f, position.z));

		//�폜
		player->SetDestroy();	//�v���C���[
		SetDestroy();			//�e�̍폜
		return;
	}

}


