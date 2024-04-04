#pragma once
#include "gameObject.h"
class TitleCamera :public GameObject
{
private:

	DirectX::XMFLOAT3	m_Target{};			//ƒJƒƒ‰‚ÌŒü‚­•ûŒü
	DirectX::XMFLOAT4X4	m_ViewMatrix{};		
public:
	void Init() override;	//‰Šú‰»
	void Draw() override;	//•`‰æ

	DirectX::XMFLOAT4X4 GetViewMatrix() { return m_ViewMatrix; }
};

