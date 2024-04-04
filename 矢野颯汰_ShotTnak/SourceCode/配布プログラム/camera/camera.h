#pragma once
#include "gameObject.h"




class Camera : public GameObject
{

private:

	DirectX::XMFLOAT3	m_Target{};
	DirectX::XMFLOAT4X4	m_ViewMatrix{};

	int m_mode;
public:
	void Init() override;
	void Update() override;
	void Draw() override;

	DirectX::XMFLOAT4X4 GetViewMatrix() { return m_ViewMatrix; }
};
