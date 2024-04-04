#pragma once
#include "gameObject.h"
class TitleCamera :public GameObject
{
private:

	DirectX::XMFLOAT3	m_Target{};			//�J�����̌�������
	DirectX::XMFLOAT4X4	m_ViewMatrix{};		
public:
	void Init() override;	//������
	void Draw() override;	//�`��

	DirectX::XMFLOAT4X4 GetViewMatrix() { return m_ViewMatrix; }
};

