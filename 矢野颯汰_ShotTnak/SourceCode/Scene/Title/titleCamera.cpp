#include "titleCamera.h"
//camera.cpp
#include "manager.h"
#include "renderer.h"
#include "title.h"
//======================
//		������
//======================
void TitleCamera::Init()
{
	//�ϐ���`
	m_Position = DirectX::XMFLOAT3(0.0f, 5.0f, -10.0f);
	m_Target = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	//�J�����̌����ƍ��W
	m_Target.x = -5.0f;
	m_Target.y = 0.0f;
	m_Target.z = -5.0f;
	m_Position.x = 5.5f;
	m_Position.y = 1.5f;
	m_Position.z = 4.0f;
}
//======================
//		  �`��
//======================
void TitleCamera::Draw()
{
	//�r���[�}�g���N�X�ݒ�
	DirectX::XMFLOAT3 up = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);
	DirectX::XMStoreFloat4x4(&m_ViewMatrix,
		DirectX::XMMatrixLookAtLH(XMLoadFloat3(&m_Position),
			XMLoadFloat3(&m_Target), XMLoadFloat3(&up))
	);

	Renderer::SetViewMatrix(&m_ViewMatrix);


	//�v���W�F�N�V�����}�g���N�X�ݒ�
	DirectX::XMFLOAT4X4 projectionMatrix;
	DirectX::XMStoreFloat4x4(&projectionMatrix,
		DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(60),
		(float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 1000.0f)
	);

	Renderer::SetProjectionMatrix(&projectionMatrix);
}

