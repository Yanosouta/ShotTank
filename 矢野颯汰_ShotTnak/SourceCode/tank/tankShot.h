#pragma once
#include "component.h"
#include "gameObject.h"
#include "manager.h"
#include "player.h"
#include "scene.h"
class TankShot :
	public Component
{
public:
	void Init();
	bool Shot();

	using Component::Component;

	float face(DirectX::XMFLOAT3);
	float Pface(DirectX::XMFLOAT3);					//ÉvÉåÉCÉÑÅ[ÇÃï˚å¸
	void Vision();
private:
	Scene* scene;
	Player* player ;

	float       m_BulletTime;
	float		m_RotY;
	int m_FrameCount;
	bool m_excFlg;
	DirectX::XMFLOAT3 m_Position;
	DirectX::XMFLOAT3 m_Forward;
	DirectX::XMFLOAT3 m_Rotation;
	DirectX::XMFLOAT3 m_Normal[6] = {
		{-1.0f,  0.0f,  0.0f},
		{ 1.0f,  0.0f,  0.0f},
		{ 0.0f, -1.0f,  0.0f},
		{ 0.0f,  1.0f,  0.0f},
		{ 0.0f,  0.0f, -1.0f},
		{ 0.0f,  0.0f,  1.0f},
	};

};

