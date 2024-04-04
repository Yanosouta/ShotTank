#pragma once
#include "component.h"
#include "gameObject.h"
#include "scene.h"
#include "box.h"
#include "bullet.h"
#include "pbullet.h"
class Reflection : public Component
{
public:
	using Component::Component;

	//===========関数定義============
	void Init() override;
	void Update() override;

	bool UpCollision(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 scale);		//上の当たり判定
	bool DownCollision(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 scale);		//下の当たり判定
	bool LeftCollision(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 scale);		//左の当たり判定
	bool RightCollision(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 scale);	//右の当たり判定

	void SetBullet(Bullet*);		//弾の情報を取得
	void SetPBullet(PBullet*);		//弾の情報を取得
	
private:
	//--------各情報を取得--------
	Scene* m_Scene;
	std::vector <Box*> m_Box;
	Bullet* m_Bullet;
	PBullet* m_PBullet;
	
private:
	//==========変数定義============
	//---------ボックス------------
	DirectX::XMFLOAT3 m_BoxPos;
	DirectX::XMFLOAT3 m_BoxScale;

	//-----------弾---------------
	DirectX::XMFLOAT3 m_BulletPos;
	DirectX::XMFLOAT3 m_BulletScale;
	DirectX::XMFLOAT3 m_BulletVel;

	DirectX::XMFLOAT3 m_TouchNor;		//法線情報を格納用

	//法線情報
	DirectX::XMFLOAT3 m_Normal[6] = {
		{-1.0f,  0.0f,  0.0f},	// 左
		{ 1.0f,  0.0f,  0.0f},	// 右
		{ 0.0f, -1.0f,  0.0f},	// 下
		{ 0.0f,  1.0f,  0.0f},	// 上
		{ 0.0f,  0.0f, -1.0f},	// 前
		{ 0.0f,  0.0f,  1.0f},	// 後
	};
	//反射
	int m_ReflCount;		//反射の回数カウント
	int m_Refl;				//反射回数
};

