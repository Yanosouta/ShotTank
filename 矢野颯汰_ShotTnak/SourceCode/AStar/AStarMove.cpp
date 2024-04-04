#include "AStarMove.h"
#include "ray.h"
#include "collision.h"
#include "box.h"
#include "gameObject.h"
#include "scene.h"
#include "player.h"
#include <list>
#include <thread>
#include <future>

//================================
//				初期化
//================================
void AStarMove::Init()
{
	//AStar
	astar = m_GameObject->AddComponent<AStar>();

	//変数定義
	float x, y ,num;
	num = MAP_HEIGHT;
	int count = 0;
	for (int i = 0; i < MAP_HEIGHT; i++)
	{
		x = num * 2 ;
		for (int j = 0; j < MAP_WIDTH; j++)
		{
			y = j * 2 + 2;
			m_MapPos[i][j] = { x,y };	//目標地点の座標を格納
			//デバッグがオンの時に表示
			if (m_DebugFlg) {
				m_CBall[i][j] = scene->GetGameObject<CreateBall>();	//情報を取得
				m_CBall[i][j] = scene->AddGameObject<CreateBall>();	//表示
				m_CBall[i][j]->SetPosition(DirectX::XMFLOAT3(m_MapPos[i][j].y, 1.0f, m_MapPos[i][j].x));	//座標をセット
				m_CBall[i][j]->SetScale(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));								//サイズをセット
			}	
			count++;
		}
		num--;
	}


}
//=========================================
//		プレイヤーに向かって移動
//=========================================
bool AStarMove::Navi()
{
	//プレイヤーの情報を取得
	player = scene->GetGameObject<Player>();
	//プレイヤーがいないときは以降の処理を行わない。
	if (player == NULL)
		return false;
	//プレイヤーの座標を取得
	DirectX::XMFLOAT3 playerPos = player->GetPosition();

	//各情報を取得
	m_Position = m_GameObject->GetPosition();	//座標
	m_Rotation = m_GameObject->GetRotation();	//角度
	m_Forward = m_GameObject->GetForward();		//前方ベクトル

	//変数定義
	DirectX::XMINT2 index;	//配列の数値
	float dist;				//距離
	
	bool flg = false;		//2回目以降判定フラグ

	//一番近いポイントを格納
	DirectX::XMINT2 startIndex = NearPoint(m_Position);
	//目的地点(プレイヤーの座標)を格納
	DirectX::XMINT2 EndIndex = NearPoint(playerPos);
	
	//初期設定または別のステートから遷移した直後に実行
	if (m_InitOneFlg || m_StateFlg)
	{
		//AStarマップの生成
		astar->CreateMap();
		//目的地までのノードを求める
		astar->AStarSearch(startIndex, EndIndex);
		//目的地までのノードを格納
		m_astarList = astar->GetAStar(m_astarList);
		m_InitOneFlg = false;
		m_StateFlg = false;
	}
	else {
		flg = true;
	}

	//一度実行したら次のポイントに着くまで実行しない
	if (m_AStarFlg)//ポイントに到着
	{
		//AStarマップの生成
		//astar->CreateMap();

		//ノードを取得
		if(flg)
			m_astarList = astar->GetAStar(m_astarList);

		//ノードの一番目を取得
		auto it = m_astarList.begin();
		//ノードがある時true
		if (m_astarList.size() > 0)
		{
			//リストを一つずらす(目標地点)
			index = *++it;
			//目標地点の座標を格納
			m_PointPos = DirectX::XMFLOAT3(m_MapPos[index.y][index.x].y,
				0.0f,
				m_MapPos[index.y][index.x].x);
		}
		
		//経路の可視化
		if (m_DebugFlg) {
			//前回の経路の色を戻す
			for (int i = 0; i < MAP_HEIGHT; i++)
			{
				for (int j = 0; j < MAP_WIDTH; j++)
				{
					ReturnColor(m_CBall[i][j]);
				}
			}
			//経路の色を変える
			auto itr = m_astarList.begin();
			index = *itr;
			//目的地までのBallの色を変更
			for (int i = 0; i < m_astarList.size() - 1; i++)
			{
				ChangeColor(m_CBall[index.y][index.x]);
				index = *++itr;
			}
		}
		m_AStarFlg = false;
		m_MoveOneFlg = true;
	}
	else
	{
		//移動中に一度だけ計算を行う
		if (m_MoveOneFlg)
		{
			//一番近いポイントを格納
			DirectX::XMINT2 startIndex = NearPoint(m_PointPos);
			DirectX::XMINT2 EndIndex = NearPoint(playerPos);
			//目的地までのノードを求める
			astar->AStarSearch(startIndex, EndIndex);
		}
		m_MoveOneFlg = false;
	}
	//目的地までの角度を計算
	m_RotY = face(m_PointPos);
	//度に変換
	m_Degrees = ToDegrees(m_RotY);
	//目的地に到着後に一度補正
	if (m_OneFlg)
	{
		if (m_Degrees >= 0.0f) {
			m_Degrees += 1.5f;
		}
		else {
			m_Degrees -= 1.5f;
		}
		m_OneFlg = false;
	}
	//一番近いポイントまでの距離を更新
	dist = Dist(m_PointPos, m_Position);

	//目的地の方向に回転
	if (m_Degrees >= 2.5f || m_Degrees <= -2.5f)
	{
		if (m_Degrees >= 0.0f) {
			m_Rotation.y += ToRadian(1.5f);		//右回り
		}
		else {
			m_Rotation.y -= ToRadian(1.5f);		//左回り
		}
	}
	else {//回転する必要がなければ移動
		if (m_PointPos.x > 0.1f)
			m_Position.x += m_Forward.x * m_Speed.x;
		if (m_PointPos.z > 0.1f)
			m_Position.z += m_Forward.z * m_Speed.z;
	}
	//目的地に着いたらフラグをリセット
	if (dist < 0.2f)
	{
		m_OneFlg = true;
		m_AStarFlg = true;
	}

	//タンクとプレイヤーの間にオブジェクトがあるか判定
	Vision();		
	//間にオブジェクトがなければ実行
	if (!m_excFlg)
	{
		m_correctedShotTime += 1.0f / 60.0f;
		//プレイヤーを見つけて直ぐに弾を撃たないように時間で調整
		if (m_correctedShotTime >= 0.5f)
		{
			//初期化
			m_correctedShotTime = 0.0f;
			m_InitOneFlg = true;
			m_StateFlg = true;
			m_AStarFlg = true;
		}

	}
	//タンクの情報をセット
	m_GameObject->SetPosition(m_Position);	//座標
	m_GameObject->SetRotation(m_Rotation);	//角度

	return m_StateFlg;
}
//===================================
//		内積で角度を求める
//===================================
float AStarMove::face(DirectX::XMFLOAT3 targetPos)
{
	//=====変数定義=====
	DirectX::XMFLOAT3 TargetDir;	//目的地までの距離
	DirectX::XMVECTOR DirVector;	//正規化した距離
	float rotY = 0.0f;				//角度
	DirectX::XMFLOAT3 forward = m_Forward;	//前方ベクトル
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
//==========================================
//一番近いポイントを求める
//==========================================
DirectX::XMINT2 AStarMove::NearPoint(DirectX::XMFLOAT3 pos)
{
	//変数定義
	float map[MAP_HEIGHT][MAP_WIDTH];	//マップ
	float temp = 5.0f;					//距離の初期値
	int num = MAP_HEIGHT;				//縦
	//配列に格納された座標までの距離を求める
	for (int i = 0; i < MAP_HEIGHT; i++)
	{
		num--;
		for (int j = 0; j < MAP_WIDTH; j++)
		{
			//距離を求める
			map[num][j] = Dist(DirectX::XMFLOAT3(m_MapPos[num][j].y, 0.0f, m_MapPos[num][j].x), pos);
			//求める数値の方が小さければ実行
			if (map[num][j] < temp)
			{
				temp= map[num][j];			//置き換える
				m_MapIndex = { j,num };
			}
		}
	}
	//一番近いポイントを返す
	return m_MapIndex;
}
//==================================================================
//			タンクとプレイヤーの間にオブジェクトがあるか判定
//==================================================================
void AStarMove::Vision()
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
