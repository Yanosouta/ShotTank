// Tankオブジェクト [tank.cpp]
#include "tank.h"
#include <math.h>
#include "shader.h"
#include "modelRenderer.h"
#include "bullet.h"
#include "box.h"
#include "input.h"
#include "ray.h"
#include "collision.h"
#include "CreateLine.h"
#include "alongthewall.h"
#include "tankMove.h"
#include "tankShot.h"
#include "tankAvoid.h"

//time
#include <time.h>

#include "XMFLOAT_Helper.h"

//ImGui
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"
//===================================
//				初期化
//===================================
void Tank::Init()
{
	//コンポーネントの追加
	AddComponent<Shader>()->Load("shader/vertexLightingVS.cso",			//シェーダ
		"shader/vertexLightingPS.cso");
	AddComponent<ModelRenderer>()->Load("asset/model/ETank.obj");		//モデル					
	AddComponent<AlongTheWall>()->SetTank(this);						//当たり判定
	tankMove = AddComponent<TankMove>();								//移動
	tankShot = AddComponent<TankShot>();								//弾を撃つ
	tankAvoid = AddComponent<TankAvoid>();								//逃げる
	astar = AddComponent<AStarMove>();									//AStar
	//初期化
	m_avertDelay = 3.0f;	//再度撃てるようになるまでの時間
	m_avertCount = 3.0f;	//始まった直後でも弾を撃つように調整
	m_hitbullet.hit = false;
	
}
//===================================
//				更新
//===================================
void Tank::Update()
{
	//プレイヤーがいなければ以降の処理を行わない
	if (player == NULL)
		return;
	//カウント
	m_avertCount += 1.0f / 60.0f;
	//前方向ベクトルの取得
	m_Forward = GetForward();
	//Rotationの補正
	if (m_Rotation.y > 6.28f)
		m_Rotation.y -= 6.28f;
	if (m_Rotation.y < 0.0f)
		m_Rotation.y += 6.28f;
	//ステートの遷移
	switch (m_currentState)
	{
	case Tank::donot:	//待機
		DoNot();
		break;
	case Tank::move:	//移動
		Move();
		break;
	case Tank::shot:	//射撃
		Shot();
		break;
	case Tank::avoid:	//逃げる
		Avoid();
		break;
	default:
		break;
	}

	//-----ImGui-----
	if (DebugFlg)	//オンの時のみ表示
	{
		ImGui::Begin("Tank");
		ImGui::Text("Pos: x:%.2f,y:%.2f,z:%.2f", m_Position.x, m_Position.y, m_Position.z);
		ImGui::Text("Rotation: x:%.2f,y:%.2f,z:%.2f", m_Rotation.x, m_Rotation.y, m_Rotation.z);
		ImGui::Text("RotY: %.2f", m_RotY);
		ImGui::Text("State: %s", statenamu[m_currentState]);
		ImGui::End();
	}
}
//===================================
//				待機
//===================================
void Tank::DoNot()
{
	//タンクとプレイヤーの間にオブジェクトがあるか判定
	Vision();
	
	//間にオブジェクトがなければ弾を撃つ
	if (!m_excFlg)
	{
		if (m_avertCount >= m_avertDelay)
		{
			//弾があたりそうか判定
			Possibility();
			{
				if (m_hitbullet.hit)
				{
					ChangeState(Tank::avoid);	//逃げる
					return;
				}
			}
		}
		ChangeState(Tank::shot);			//弾を撃つ
	}
	else
	{
		ChangeState(Tank::move);			//移動
	}

}
//===================================
//				移動
//===================================
void Tank::Move()
{
	bool moveflg = false;

	moveflg = astar->Navi();		//フラグを取得
	//trueの時遷移
	if (moveflg)					
		ChangeState(Tank::donot);	//待機に遷移
}
//===================================
//				弾を撃つ
//===================================
void Tank::Shot()
{
	bool shotFlg = false;
	shotFlg = tankShot->Shot();		//フラグを取得
	//trueの時遷移
	if(shotFlg)
		ChangeState(Tank::donot);	//待機に遷移
}
//===================================
//				逃げる
//===================================
void Tank::Avoid()
{
	bool avoidFlg = false;
	avoidFlg = tankAvoid->Avoid();		//フラグを取得
	//trueの時遷移
	if (avoidFlg)
		ChangeState(Tank::donot);		//待機に遷移
}
//===================================
//		内積で角度を求める
//===================================
float Tank::face(DirectX::XMFLOAT3 targetPos)
{
	//=====変数定義=====
	DirectX::XMFLOAT3 TargetDir;	//目的地までの距離
	DirectX::XMVECTOR DirVector;	//正規化した距離
	float rotY = 0.0f;				//角度
	DirectX::XMFLOAT3 forward = GetForward();	//前方ベクトル
	//位置ベクトルの計算
	TargetDir = DirectX::XMFLOAT3(targetPos.x - m_Position.x, targetPos.y - m_Position.y, targetPos.z - m_Position.z);
	//正規化
	DirVector = DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&TargetDir));
	//内積の計算
	//90°補正
	DirectX::XMMATRIX  rotMat = DirectX::XMMatrixRotationY(3.14f / 2);
	DirectX::XMStoreFloat3(&forward, DirectX::XMVector3Normalize(DirectX::XMVector3Transform(DirectX::XMLoadFloat3(&forward), rotMat)));

	//内積の計算
	DirectX::XMStoreFloat(&rotY, DirectX::XMVector3Dot(
		DirectX::XMVector3Normalize(DirVector),
		DirectX::XMLoadFloat3(&forward)));

	return rotY;
}
//==================================================================
//			タンクとプレイヤーの間にオブジェクトがあるか判定
//==================================================================
void Tank::Vision()
{
	//======変数定義======
	Ray::Result result;			//Rayの結果を格納
	Ray::Result exclusion;		//Rayの結果を格納
	Ray::Triangle triangle[2];	//頂点情報を格納
	Collision::Vertices vtx;	//頂点情報
	DirectX::XMFLOAT3 point;	//Rayの当たった座標
	m_excFlg = false;

	DirectX::XMFLOAT3  playerPos =	//プレイヤーの座標
		player->GetPosition();
	DirectX::XMFLOAT3 TargetDir =	//プレイヤーとタンクの距離
		DirectX::XMFLOAT3(playerPos.x - m_Position.x, 
						  playerPos.y - m_Position.y, 
						  playerPos.z - m_Position.z);
	//ベクトルの計算
	float Dis = sqrtf(TargetDir.x * TargetDir.x + TargetDir.z * TargetDir.z);
	//プレイヤーまでレイを飛ばす
	for (int i = 0; i < 6; i++)
	{
		//Y軸を除外
		if (i != 2 && i != 3)
		{
			//シーン内にあるBoxの情報を取得
			std::vector<Box*> boxList = scene->GetGameObjects<Box>();
			//Boxの数だけ回す
			for (Box* box : boxList)
			{
				//法線の数だけ回す
				for (int j = 0; j < 6; j++)
				{
					DirectX::XMFLOAT3 boxPos = box->GetPosition();	//座標
					DirectX::XMFLOAT3 boxSize = box->GetScale();	//サイズ
					//法線の方向のサイズを加える
					if (j == 0)
						boxPos.x -= boxSize.x;	//-X方向
					if (j == 1)
						boxPos.x += boxSize.x;	// X方向
					if (j == 4)
						boxPos.z -= boxSize.z;	//-Z方向
					if (j == 5)
						boxPos.z += boxSize.z;	// Z方向
					//法線からY軸を除外
					if (j != 2 && j != 3)
					{
						//レイの計算
						exclusion = Ray::CheckRayPlane(m_Normal[j], boxPos, m_Position, TargetDir, Dis + 0.1f);
						//レイが当たった面があればtrue
						if (exclusion.hit)
						{
							//頂点情報を取得
							vtx = box->GetComponent<Collision>()->GetVertices();
							//法線各面の頂点情報を格納
							switch (j)
							{		//三角形2つで１つの面
							case 0:
								triangle[0] = { vtx.pos[0],vtx.pos[2],vtx.pos[4] };
								triangle[1] = { vtx.pos[2],vtx.pos[6],vtx.pos[4] };
								break;
							case 1:
								triangle[0] = { vtx.pos[3],vtx.pos[1],vtx.pos[7] };
								triangle[1] = { vtx.pos[1],vtx.pos[5],vtx.pos[7] };
								break;
							case 4:
								triangle[0] = { vtx.pos[2],vtx.pos[3],vtx.pos[6] };
								triangle[1] = { vtx.pos[3],vtx.pos[7],vtx.pos[6] };
								break;
							case 5:
								triangle[0] = { vtx.pos[1],vtx.pos[0],vtx.pos[5] };
								triangle[1] = { vtx.pos[0],vtx.pos[4],vtx.pos[5] };
								break;
							default:
								break;
							}
							//レイが当たった座標を保管
							point = exclusion.point;
							//三角形２つ
							for (int j = 0; j < 2; j++)
							{
								//面の範囲外で判定しないように面の範囲内なのか判定
								result = Ray::CheckPointTraiangle(point, triangle[j]);
								//範囲内であればtrue
								if (result.hit)
								{
									//範囲内であれば別の方向までスキップ
									m_excFlg = true;
									break;
								}
							}
							if (m_excFlg)
								break;
						}
					}

				}
				if (m_excFlg)
					break;
			}
			if (m_excFlg)
				break;
		}
	}
}
//===================================
//		敵が弾を視認できているか判定
//===================================
void Tank::Possibility()
{
	//処理をする前に初期化
	m_hitbullet.dot = 0.0f;
	m_hitbullet.hit = false;
	m_hitbullet.pos = { 0.0f,0.0f,0.0f };
	m_hitbullet.velo = {0.0f,0.0f,0.0f};
	//変数定義
	Ray::Result result;			
	Ray::Result exclusion;		
	Ray::Triangle triangle[2];
	Collision::Vertices vtx;
	DirectX::XMFLOAT3 point;
	std::vector<PBullet*> pbullet;	//弾の情報
	float rotY = 0.0f;

	bool checkFlg = false;

	//弾の情報をシーン内から取得
	pbullet = scene->GetGameObjects<PBullet>();
	//シーン内にある弾の数回す。
	for (PBullet* bullet : pbullet)
	{
		DirectX::XMFLOAT3 bulletVec = bullet->GetVelocity();	//弾のベクトル
		DirectX::XMFLOAT3 bulletPos = bullet->GetPosition();	//弾の座標
		DirectX::XMFLOAT3 TargetDir =							//弾とタンクの距離
			DirectX::XMFLOAT3(bulletPos.x - m_Position.x, 
							  bulletPos.y - m_Position.y, 
							  bulletPos.z - m_Position.z);
		//ベクトルの計算
		float Dis = sqrtf(TargetDir.x * TargetDir.x + TargetDir.z * TargetDir.z);
		//シーン内のboxを取得
		std::vector<Box*> boxList = scene->GetGameObjects<Box>();
		//一定距離より近い時のみ処理
		if (Dis <= 15.0f)
		{
			//boxの数回す。
			for (Box* box : boxList)
			{
				//法線の数だけ回す
				for (int k = 0; k < 6; k++)
				{

					DirectX::XMFLOAT3 boxPos = box->GetPosition();	//座標
					DirectX::XMFLOAT3 boxSize = box->GetScale();	//サイズ

					if (k == 0)
						boxPos.x -= boxSize.x;
					if (k == 1)
						boxPos.x += boxSize.x;
					if (k == 4)
						boxPos.z -= boxSize.z;
					if (k == 5)
						boxPos.z += boxSize.z;
					//Y軸を除外
					if (k != 2 && k != 3)
					{
						exclusion = Ray::CheckRayPlane(m_Normal[k], boxPos, m_Position, TargetDir, Dis + 0.1f);
						if (exclusion.hit)
						{
							vtx = box->GetComponent<Collision>()->GetVertices();
							switch (k)
							{
							case 0:
								triangle[0] = { vtx.pos[0],vtx.pos[2],vtx.pos[4] };
								triangle[1] = { vtx.pos[2],vtx.pos[6],vtx.pos[4] };
								break;
							case 1:
								triangle[0] = { vtx.pos[3],vtx.pos[1],vtx.pos[7] };
								triangle[1] = { vtx.pos[1],vtx.pos[5],vtx.pos[7] };
								break;
							case 4:
								triangle[0] = { vtx.pos[2],vtx.pos[3],vtx.pos[6] };
								triangle[1] = { vtx.pos[3],vtx.pos[7],vtx.pos[6] };
								break;
							case 5:
								triangle[0] = { vtx.pos[1],vtx.pos[0],vtx.pos[5] };
								triangle[1] = { vtx.pos[0],vtx.pos[4],vtx.pos[5] };
								break;
							default:
								break;
							}

							point = exclusion.point;
							for (int j = 0; j < 2; j++)
							{
								result = Ray::CheckPointTraiangle(point, triangle[j]);
								//障害物が間にあればtrue
								if (result.hit)
								{
									checkFlg = true;
									break;
								}
							}
							if (checkFlg)
								break;
						}
					}
				}
				if (checkFlg)
					break;
			}
			//弾とタンクの間にオブジェクトが無い かつステートがavoidの場合
			if (!checkFlg && Tank::avoid)
			{
				//弾とタンクの角度
				rotY = face(bulletPos);
				//弾を感知する範囲
				if (rotY <= 0.3f && rotY >= -0.3f)
				{
					//当たりそうか内積を使って計算
					m_hitbullet = Dot(bulletVec,bulletPos);
					//デバッグがオンの時に実行
					if (DebugFlg)
					{
						//========弾の色を変える==========
						ChangeColor(bullet);
						//===============================
					}
					
					break;
				}
			}
		}
	}
}
//===================================
//		内積の結果をHitBulletに格納
//===================================
Tank::HitBullet Tank::Dot(DirectX::XMFLOAT3 vec,DirectX::XMFLOAT3 pos)
{
	//構造体宣言
	HitBullet hit;					
	//変数定義
	DirectX::XMFLOAT3 bulletVelo;	//弾の速度
	DirectX::XMFLOAT3 bulletdis;	//距離
	float dot;						//計算結果
	//正規化
	DirectX::XMStoreFloat3(&bulletVelo, DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&vec)));

	//弾までの距離
	bulletdis = pos - m_Position;
	//正規化
	DirectX::XMStoreFloat3(&bulletdis, DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&bulletdis)));
	//内積の計算
	DirectX::XMStoreFloat(&dot, DirectX::XMVector3Dot(
		DirectX::XMLoadFloat3(&bulletVelo),
		DirectX::XMLoadFloat3(&bulletdis)));
	//再度範囲内か判定
	if (dot <= -0.96)
	{
		//計算結果を格納
		hit.hit = true;
		hit.pos = pos;
		hit.velo = bulletVelo;
		hit.dot = dot;
	}
	else
	{
		//範囲外の時に初期化
		hit.hit = false;
		hit.pos = {0.0f,0.0f,0.0f};
		hit.velo = { 0.0f,0.0f,0.0f };
		hit.dot = 0.0f;
	}
	return hit;
}

//===========================================
//			弾の色を変える
//===========================================
void Tank::ChangeColor(PBullet * bullet)
{
	//弾の色を変える
	bullet->ChangeColor();
}

