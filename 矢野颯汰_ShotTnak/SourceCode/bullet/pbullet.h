// pBulletオブジェクト [pbullet.h]
#pragma once
#include "gameObject.h"
#include "component.h"
#include "scene.h"


class PBullet : public GameObject
{
private:
	Scene* scene;
	float m_length = 0.0f;

public:
	void Init() override;	//初期化
	void Update() override;	//更新
	
	//ベクトルをセット
	void SetVelocity(DirectX::XMFLOAT3 Velocity) { m_Velocity = Velocity; }
	//ベクトルを取得
	DirectX::XMFLOAT3 GetVelocity() { return m_Velocity; }

	
	//色を変える
	void ChangeColor();


};

