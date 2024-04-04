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
	DirectX::XMFLOAT3 m_fcross;		//法線との判定で使用
	DirectX::XMFLOAT3 m_Speed;		//プレイヤーの速さ
	bool m_collisionFlg;			//当たっているか判定
	//法線ベクトル
	DirectX::XMFLOAT3 m_Normal[6] = {
		{-1.0f,  0.0f,  0.0f},	// 左
		{ 1.0f,  0.0f,  0.0f},	// 右
		{ 0.0f, -1.0f,  0.0f},	// 下
		{ 0.0f,  1.0f,  0.0f},	// 上
		{ 0.0f,  0.0f, -1.0f},	// 前
		{ 0.0f,  0.0f,  1.0f},	// 後
	};
};

