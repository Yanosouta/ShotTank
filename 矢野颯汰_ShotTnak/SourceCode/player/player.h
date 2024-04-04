#pragma once

#include "gameObject.h"

class Player : public GameObject
{
public:
	//==========�֐���`=============
	void Init() override;
	void Update() override;
private:
	
	class Audio* m_SE{};
private:
	//==========�ϐ���`==============
	DirectX::XMFLOAT3 m_Velocity{}; //���x
	//�f�B���C
	float m_Delay;		//�f�B���C�̎���
	float m_DelayCount;	//�J�E���g�p
	//�f�o�b�O
	bool m_DebugFlg = false;
	

};
