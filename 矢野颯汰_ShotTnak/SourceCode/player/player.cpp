#include "player.h"
#include "manager.h"
#include "renderer.h"
#include "cube.h"
#include "shader.h"
#include "input.h"
#include "modelRenderer.h"
#include "pbullet.h"
#include "box.h"
#include "explosion.h"
#include "audio.h"
#include "reflection.h"
#include "alongthewall.h"

//ImGui
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"


//===================================
//				������
//===================================
void Player::Init()
{
	//----------�R���|�[�l���g�擾--------------
	AddComponent<Shader>()->Load("shader/vertexLightingVS.cso", "shader/vertexLightingPS.cso");//�V�F�[�_
	AddComponent<ModelRenderer>()->Load("asset/Model/PTank.obj");							   //���f��
	AddComponent<AlongTheWall>()->SetPlayer(this);											   //�����蔻��
	m_SE = AddComponent<Audio>();															   //SE
	//-------------SE�擾-----------------
	m_SE->Load("asset/audio/BulletSound.wav");												   

	//--------------������-------------
	SetPosition(DirectX::XMFLOAT3(0.0f, 0.5f, 0.0f));	//�v���C���[�̃T�C�Y��ݒ�
	m_Delay = 1.5f;										//�ēx�e�����Ă�悤�ɂ��鎞�Ԃ�ݒ�
	m_DelayCount = 1.5f;								//�v���C����ɒe�����Ă�悤�ɐݒ�
	m_Speed = { 0.1f,0.1f,0.1f };						//�ړ����x��ݒ�
}
//===================================
//				�X�V
//===================================
void Player::Update()
{
	//�V�[���̏����擾
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();

	//�J�E���g
	m_DelayCount += 1.0f / 60.0f;

	//�v���C���[�����Ȃ���Έȍ~�̏������s��Ȃ�
	if (player == NULL)
		return;
	//�v���C���[�̌����Ă���������擾
	DirectX::XMFLOAT3 forward = GetForward();
	//"A"�����������Ă���ԍ���]������
	if (Input::GetKeyPress('A'))
	{
		m_Rotation.y -= 0.05f;
	}
	//"D"�����������Ă���ԉE��]������
	if (Input::GetKeyPress('D'))
	{
		m_Rotation.y += 0.05f;
	}
	//"W"�����������Ă���ԑO�ɐi��
	if (Input::GetKeyPress('W'))
	{
		m_Position.x += forward.x * m_Speed.x;
		m_Position.z += forward.z * m_Speed.z;
	}
	//"S"�����������Ă���Ԍ��ɐi��
	if (Input::GetKeyPress('S'))
	{
		m_Position.x -= forward.x * m_Speed.x;
		m_Position.z -= forward.z * m_Speed.z;
	}
	
	//�d��
	m_Velocity.y -= 0.015f;

	//��R
	m_Velocity.y -= m_Velocity.y * 0.01f;

	//�ړ�
	m_Position.x += m_Velocity.x;
	m_Position.z += m_Velocity.z;

	// �e����
	//�e����������Ԋu���󂯂�
	if (m_DelayCount >= m_Delay)
	{
		//"K"����������e������
		if (Input::GetKeyTrigger('K'))
		{
			//�e�̏����擾
			PBullet* bullet = scene->AddGameObject<PBullet>();
			//�������W���Z�b�g
			bullet->SetPosition(DirectX::XMFLOAT3(m_Position.x + 0.0f,
				m_Position.y + 1.0f, m_Position.z + 0.0f));
			//�X�s�[�h��������
			bullet->SetVelocity(DirectX::XMFLOAT3(forward.x * 0.15f,
				forward.y * 0.15f, forward.z * 0.15f));
			
			//SE
			m_SE->Play();
			//�J�E���g�����Z�b�g����
			m_DelayCount = 0.0f;
		}
	}
	//�f�o�b�O���I���̎��ɕ\��
	if (m_DebugFlg)
	{
		ImGui::Begin("PlayerPos");
		ImGui::Text("PlayerPos: x:%.2f,y:%.2f,z:%.2f", m_Position.x, m_Position.y, m_Position.z);
		ImGui::End();
	}
}





