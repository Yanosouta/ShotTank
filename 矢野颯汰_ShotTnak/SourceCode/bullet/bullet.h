// Bulletオブジェクト [bullet.h]
#pragma once
#include "gameObject.h"
#include "scene.h"
#include "player.h"
class Bullet : public GameObject
{
public:
	void Init() override;		//初期化
	void Update() override;		//更新
	//当たり判定
	void collision();	
	//弾のベクトルをセット
	void SetVelocity(DirectX::XMFLOAT3 Velocity) { m_Velocity = Velocity; }	
	//弾のベクトルを取得
	DirectX::XMFLOAT3 GetVelocity() { return m_Velocity; }
	
private:
	Scene* scene;
	Player* player;
private:

	float m_length = 0.0f;//長さ

};

