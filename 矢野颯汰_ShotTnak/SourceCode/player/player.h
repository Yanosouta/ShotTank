#pragma once

#include "gameObject.h"

class Player : public GameObject
{
public:
	//==========関数定義=============
	void Init() override;
	void Update() override;
private:
	
	class Audio* m_SE{};
private:
	//==========変数定義==============
	DirectX::XMFLOAT3 m_Velocity{}; //速度
	//ディレイ
	float m_Delay;		//ディレイの時間
	float m_DelayCount;	//カウント用
	//デバッグ
	bool m_DebugFlg = false;
	

};
