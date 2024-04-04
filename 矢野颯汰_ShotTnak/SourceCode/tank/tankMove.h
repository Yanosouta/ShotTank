#pragma once
#include "component.h"
#include "gameObject.h"
#include "scene.h"
#include "player.h"
#include "manager.h"
#include "CreateBall.h"

#define TARGETPOS 12	//目的地の数
#define ROWS 3			//縦
#define COLS 4			//横
#define DELAY 1.0f		//再行動までの時間

class TankMove : public Component
{
public:
	using Component::Component;

	void Init() override;	//初期化
	bool Navi();			//移動
	//内積で角度を求める
	float face(DirectX::XMFLOAT3);
	//タンクとプレイヤーの間にオブジェクトがあるか判定
	void Vision();
	//配列の位置を求める
	void ArrayPoint();
	//2次元配列の隣接した要素を求める
	void SelectAdjacent(int row, int col);
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
	//移動間にオブジェクトがあるか判定
	void MovePossibility();

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
	Scene* scene = Manager::GetScene();					//シーンの情報を取得
	Player* player = scene->GetGameObject<Player>();	//プレイヤーの情報を取得

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
	//目的地
	DirectX::XMFLOAT3 m_TargetPos[12] = {
		{2.0f,0.0f,2.0f},		//0
		{11.0f,0.0f,2.0f},		//1
		{23.0f,0.0f,2.0f},		//2
		{33.0f,0.0f,2.0f},		//3
		{2.0f,0.0f,10.0f},		//4
		{11.0f,0.0f,10.0f},		//5
		{23.0f,0.0f,10.0f},		//6
		{33.0f,0.0f,10.0f},		//7
		{2.0f,0.0f,18.0f},		//8
		{11.0f,0.0f,18.0f},		//9
		{23.0f,0.0f,18.0f},		//10
		{33.0f,0.0f,18.0f},		//11
	};
	//マップ情報
	int m_TargetPosArray[ROWS][COLS] = {
		{8, 9,10,11},
		{4, 5, 6, 7},
		{0, 1, 2, 3},
	};
	//隣接した配列を格納
	int m_AbjacentPos[4] = { 0,0,0,0 };


	bool m_MoveFlg;						//移動判定フラグ
	bool m_DirObj[4];					//移動方向判定フラグ
	bool m_OneFlg = true;				//一度実行フラグ
	bool m_excFlg;						//計算フラグ
	float m_Distance[TARGETPOS];		//距離
	float m_Degrees;					//度
	float m_RotY;						//角度
	float m_Delay;						//遅延する時間
	float m_DelayCount;					//遅延カウント
	float m_correctedShotTime = 0.0f;	//直ぐに弾を打たないように補正する
	int m_randPos = 0;					//randの値を格納
	int m_FrameCount;					//カウント
	
	DirectX::XMFLOAT3 m_Position;		//座標
	DirectX::XMFLOAT3 m_Forward;		//前方ベクトル
	DirectX::XMFLOAT3 m_Rotation;		//角度
	DirectX::XMFLOAT3 m_Speed = 		//速度
		DirectX::XMFLOAT3(0.1f, 0.1f, 0.1f);

	//デバッグ用
	bool m_DebugFlg = false;
	CreateBall* m_CBall[TARGETPOS] ;
	
};

