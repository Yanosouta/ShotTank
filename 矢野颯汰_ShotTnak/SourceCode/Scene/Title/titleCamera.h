#pragma once
#include "gameObject.h"
class TitleCamera :public GameObject
{
private:

	DirectX::XMFLOAT3	m_Target{};			//カメラの向く方向
	DirectX::XMFLOAT4X4	m_ViewMatrix{};		
public:
	void Init() override;	//初期化
	void Draw() override;	//描画

	DirectX::XMFLOAT4X4 GetViewMatrix() { return m_ViewMatrix; }
};

