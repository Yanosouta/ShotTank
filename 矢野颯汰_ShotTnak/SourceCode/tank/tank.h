// Tankオブジェクト [tank.h]
#pragma once
#include "gameObject.h"
#include "manager.h"
#include "player.h"
#include "pbullet.h"
#include "tankMove.h"
#include "tankShot.h"
#include "tankAvoid.h"
#include "CreateBall.h"
#include "AStarMove.h"
#include <vector>

class Tank : public GameObject
{
private:
	//=======構造体=========
	//rayの戻り値
	struct Result
	{
		bool hit;
		DirectX::XMFLOAT3 point;
	};
	//当たりそうな弾の情報
	struct HitBullet
	{
		bool hit;					//当たったか判定
		DirectX::XMFLOAT3 pos;		//座標
		DirectX::XMFLOAT3 velo;		//速度
		float dot;					//内積結果
	};

	//=======列挙型=========
	//State
	enum State
	{
		donot,	//待機
		move,	//移動
		shot,	//撃つ
		avoid,	//避ける
	};

	//=======変数定義=======
	//-----XMFLOAT3-----
	//法線
	DirectX::XMFLOAT3 m_Normal[6] = {		
		{-1.0f,  0.0f,  0.0f},	
		{ 1.0f,  0.0f,  0.0f},	
		{ 0.0f, -1.0f,  0.0f},	
		{ 0.0f,  1.0f,  0.0f},	
		{ 0.0f,  0.0f, -1.0f},	
		{ 0.0f,  0.0f,  1.0f},	
	};  
	//-----float-----
	float m_RotY;						//角度
	float m_avertDelay;					//再度逃げるようになるまでの時間
	float m_avertCount;					//カウント用
	
	//-----bool-----
	bool m_excFlg;
	
	//ステート
	State m_currentState = State::donot;	//現在のステートを格納
	
	//デバッグ用
	bool DebugFlg = false;
	const char* statenamu[4] = {	//現在のステートを表示用
	"donot",
	"move",
	"shot",
	"avoid"
	};  
	
private:
	//情報を取得											
	Scene* scene = Manager::GetScene();					//シーン
	Player* player = scene->GetGameObject<Player>();	//プレイヤー
	//各ステートを取得									
	TankShot* tankShot;									//弾を撃つ
	TankAvoid* tankAvoid;								//逃げる
	TankMove* tankMove;									//移動(ランダム)
	AStarMove* astar;									//移動(AStar)

	HitBullet m_hitbullet;	//宣言
	
public:
	//=========関数定義==========
	void Init() override;		//初期化
	void Update() override;		//更新

	//ステート
	void DoNot();		//待機
	void Move();		//移動
	void Shot();		//弾を撃つ
	void Avoid();		//逃げる
	//内積で角度を求める
	float face(DirectX::XMFLOAT3);
	//タンクとプレイヤーの間にオブジェクトがあるか判定
	void Vision();
	//敵が弾を視認できているか判定
	void Possibility();
	//内積結果をHitBulletに格納
	HitBullet Dot(DirectX::XMFLOAT3 vec,DirectX::XMFLOAT3 pos);
	//ステートを変える
	void ChangeState(State newState) { m_currentState = newState; }
	//ラジアンから度に変換
	float ToDegrees(float rad) { return rad * (180.0f / 3.1415f);}
	//度からラジアンに変換
	float ToRadian(float degrees) {return degrees * (3.1415f / 180.0f); }
	DirectX::XMFLOAT3 GetForward()//前方向ベクトル取得
	{
		DirectX::XMFLOAT4X4 rot;
		DirectX::XMStoreFloat4x4(&rot,
			DirectX::XMMatrixRotationRollPitchYaw(
				m_Rotation.x, m_Rotation.y, m_Rotation.z));

		DirectX::XMFLOAT3 forward;
		forward.x = rot._31;
		forward.y = rot._32;
		forward.z = rot._33;

		return forward;
	}
	//デバッグ用
	void ChangeColor(PBullet* bullet);
};

