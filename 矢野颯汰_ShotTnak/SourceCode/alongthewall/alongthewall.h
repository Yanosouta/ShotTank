#pragma once
#include "gameObject.h"
#include "box.h"
#include "player.h"
#include "tank.h"
#include "scene.h"
class AlongTheWall : public Component
{
public:
	void Init() override;
	void Update() override;

	using Component::Component;

	void SetPlayer(Player*);
	void SetTank(Tank*);
	bool UpCollision(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 scale);
	bool DownCollision(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 scale);
	bool LeftCollision(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 scale);
	bool RightCollision(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 scale);
private:
	std::vector <Box*> m_Box;
	Scene* m_Scene;
	Player* m_Player;
	Tank* m_Tank;
	DirectX::XMFLOAT3 m_BoxPos;
	DirectX::XMFLOAT3 m_BoxScale;
	DirectX::XMFLOAT3 m_Pos;
	DirectX::XMFLOAT3 m_Scale;
	DirectX::XMFLOAT3 m_Velo;
	DirectX::XMFLOAT3 m_TouchNor;
	DirectX::XMFLOAT3 m_fcross;		//�@���Ƃ̔���Ŏg�p
	DirectX::XMFLOAT3 m_Speed;		//�v���C���[�̑���
	bool m_collisionFlg;			//�������Ă��邩����
	//�@���x�N�g��
	DirectX::XMFLOAT3 m_Normal[6] = {
		{-1.0f,  0.0f,  0.0f},	// ��
		{ 1.0f,  0.0f,  0.0f},	// �E
		{ 0.0f, -1.0f,  0.0f},	// ��
		{ 0.0f,  1.0f,  0.0f},	// ��
		{ 0.0f,  0.0f, -1.0f},	// �O
		{ 0.0f,  0.0f,  1.0f},	// ��
	};
};

