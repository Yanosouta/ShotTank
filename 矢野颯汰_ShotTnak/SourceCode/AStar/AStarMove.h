#pragma once
#include "component.h"
#include "gameObject.h"
#include "scene.h"
#include "player.h"
#include "manager.h"
#include "AStar.h"
#include "CreateBall.h"


#define MAP 153		//目的地の数

class AStarMove :public Component
{
public:
	using Component::Component;
	//初期化
	void Init() override;	
	//プレイヤーまで移動
	bool Navi();			
	//内積で角度を求める
	float face(DirectX::XMFLOAT3);
	//一番近いポイントを求める
	DirectX::XMINT2 NearPoint(DirectX::XMFLOAT3 pos);
	//タンクとプレイヤーの間にオブジェクトがあるか判定
	void Vision();
	//距離を求める
	float Dist(DirectX::XMFLOAT3 pos1, DirectX::XMFLOAT3 pos2) {
		float dx = pos2.x - pos1.x;
		float dz = pos2.z - pos1.z;
		return sqrt(dx * dx + dz * dz);
	}
	//ラジアンから度に変換
	float ToDegrees(float rad) { return rad * (180.0f / 3.1415f); }
	//度からラジアンに変換
	float ToRadian(float degrees) { return degrees * (3.1415f / 180.0f); }
	
	//デバッグ用
	//色を変える
	void ChangeColor(CreateBall* ball) {
		ball->ChangeColor();
	}
	//色を初期の色に戻す
	void ReturnColor(CreateBall* ball) {
		ball->ReturnColor();
	}
private:
	//法線
	DirectX::XMFLOAT3 m_Normal[6] = {
		{-1.0f,  0.0f,  0.0f},
		{ 1.0f,  0.0f,  0.0f},
		{ 0.0f, -1.0f,  0.0f},
		{ 0.0f,  1.0f,  0.0f},
		{ 0.0f,  0.0f, -1.0f},
		{ 0.0f,  0.0f,  1.0f},
	};


private:
	Scene* scene = Manager::GetScene();	//シーン
	Player* player;		//プレイヤー
	
	AStar* astar;		//AStar
	CreateBall* m_CBall[MAP_HEIGHT][MAP_WIDTH];	//デバッグ用ボール

private:
	//変数定義
	bool m_excFlg;				//見つけたか判定フラグ
	bool m_OneFlg = true;		//回転するか判定フラグ
	bool m_MoveOneFlg = true;	//移動中に一度だけ実行
	bool m_InitOneFlg = true;	//初期設定フラグ
	bool m_AStarFlg = true;		//AStarの計算を行うか判定フラグ
	bool m_StateFlg = true;		//ステートを遷移させるか判定フラグ

	float m_Degrees;					//度
	float m_RotY;						//角度
	float m_correctedShotTime = 0.0f;	//直ぐに弾を打たないように補正する

	//マップ
	DirectX::XMINT2 m_MapIndex;							//配列の添え字を格納
	DirectX::XMFLOAT2 m_MapPos[MAP_HEIGHT][MAP_WIDTH];	//目的地の座標を格納(FLOAT2)
	DirectX::XMFLOAT3 m_PointPos;						//目的地の座標を格納(FLOAT3)

	DirectX::XMFLOAT3 m_Position;						//座標
	DirectX::XMFLOAT3 m_Forward;						//前方ベクトル
	DirectX::XMFLOAT3 m_Rotation;						//角度
	DirectX::XMFLOAT3 m_Speed =							//移動速度
		DirectX::XMFLOAT3(0.1f, 0.1f, 0.1f);

	//list
	std::list<DirectX::XMINT2> m_astarList;		//プレイヤーまでのノードを格納

	//デバッグ用
	bool m_DebugFlg = false;
};


