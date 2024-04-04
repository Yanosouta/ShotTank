#pragma once
#include "component.h"
#include "scene.h"
#include "pbullet.h"
class TankAvoid :public Component
{
private:
	Scene* scene;					//シーン
	std::vector<PBullet*> pbullet;	//弾
	//当たりそうな弾の情報
	struct HitBullet
	{
		bool hit;						//当たりそうか判定
		DirectX::XMFLOAT3 pos;			//座標
		DirectX::XMFLOAT3 velo;			//ベクトル
		float dot;						//内積の結果
	};
	HitBullet m_HitBullet;				//構造体宣言
	DirectX::XMFLOAT3 m_Position;		//座標
	DirectX::XMFLOAT3 m_Forward;		//前方ベクトル
	DirectX::XMFLOAT3 m_Rotation;		//角度
	DirectX::XMFLOAT3 m_Speed;			//速度
	DirectX::XMFLOAT3 m_RayDir[4];		//レイの飛ばす方向
	DirectX::XMFLOAT3 m_Dir4[4] = {		//4方向
		{-1.0f,  0.0f,  0.0f},
		{ 1.0f,  0.0f,  0.0f},
		{ 0.0f,  0.0f, -1.0f},
		{ 0.0f,  0.0f,  1.0f},
	};
	DirectX::XMFLOAT3 m_FacePoint[4] = {	//レイの当たった座標
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f} };
	DirectX::XMFLOAT3 m_Normal[6] = {		//法線
		{-1.0f,  0.0f,  0.0f},
		{ 1.0f,  0.0f,  0.0f},
		{ 0.0f, -1.0f,  0.0f},
		{ 0.0f,  1.0f,  0.0f},
		{ 0.0f,  0.0f, -1.0f},
		{ 0.0f,  0.0f,  1.0f},
	};
	float m_Vec[4] = { 0.0f,0.0f,0.0f,0.0f };	//ベクトル
	bool m_AvoidFlg;					//逃げる判定フラグ
	bool m_CollisionFlg;				//オブジェクトに当たった時true
	float m_PAngle;						//座標までの角度
	float m_tempAngle = 0.0f;			//↑を保管
	float m_avertDelay;					//再度逃げれるようになるまでの時間
	float m_avertCount;					//カウント
	float m_MoveTime ;			//避ける時の移動する時間
	float m_MoveCount;
	
	//デバッグ用
	bool DebugFlg = false;
	bool DebugOneFlg = true;
	float DebugTime ;
	DirectX::XMFLOAT3 DebugPos;
public:
	using Component::Component;
	
	void Init();	//初期化
	//逃げる
	bool Avoid();	
	//当たった面までの長さをソート
	void LenSort();
	//外積を使った角度の計算
	float PointAngle();
	//弾が当たりそうか判定
	void Possibility();
	//内積で角度を求める
	float face(DirectX::XMFLOAT3);
	//内積の計算
	HitBullet Dot(DirectX::XMFLOAT3 vec, DirectX::XMFLOAT3 pos);
};

