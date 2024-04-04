#include "tankMove.h"
#include "ray.h"
#include "collision.h"
#include "box.h"
#include "gameObject.h"
#include "scene.h"
#include <list>
//=======================
//		初期化
//=======================
void TankMove::Init()
{
	
	m_MoveFlg = true;		
	m_Delay = DELAY;
	m_DelayCount = DELAY;  
		
	//デバッグがオンの時目的地を表示
	if (m_DebugFlg)
	{
		for (int i = 0; i < TARGETPOS; i++)
		{
			m_CBall[i] = scene->GetGameObject<CreateBall>();
			m_CBall[i] = scene->AddGameObject<CreateBall>();
			m_CBall[i]->SetPosition(m_TargetPos[i]);
			m_CBall[i]->SetScale(DirectX::XMFLOAT3(2.0f, 2.0f, 2.0f));
		}
	}
}
//=======================
//		　移動
//=======================
bool TankMove::Navi()
{
	//変数定義
	float dist;
	bool m_StateFlg = false;
	//各情報を取得
	m_Position = m_GameObject->GetPosition();
	m_Rotation = m_GameObject->GetRotation();
	m_Forward = m_GameObject->GetForward();
	
	

	//目的地までの距離を求める
	if (m_MoveFlg)
	{
		m_MoveFlg = false;
		//各目的地までの距離を計算
		for (int i = 0; i < TARGETPOS; i++)
		{
			m_Distance[i] = Dist(m_TargetPos[i], m_Position);
		}
		//一番近い目的地を求める
		ArrayPoint();
		//4方向でオブジェクトがあるか判定する
		MovePossibility();
		//オブジェクトがある方向は除外
		for (int i = 0; i < 4; i++)
		{
			//オブジェクトがある方向には-1
			if (m_DirObj[i])
				m_AbjacentPos[i] = -1;
		}
		//オブジェクトが無い方向になるまで回す
		while (1)
		{
			m_randPos = rand() % 4;

			//ランダムの結果格納された変数が整数ならwhile文をぬける
			if (m_AbjacentPos[m_randPos] != -1)
				break;
		}

	}
	//目的地までの距離を常に更新
	dist = Dist(m_TargetPos[m_AbjacentPos[m_randPos]], m_Position);
	//目的地までの角度を計算
	m_RotY = face(m_TargetPos[m_AbjacentPos[m_randPos]]);
	//度に変換
	m_Degrees = ToDegrees(m_RotY);
	//目的地の色を変える（デバッグ用）
	if(m_DebugFlg)
		ChangeColor(m_CBall[m_AbjacentPos[m_randPos]]);
	//バグらないように1度だけ補正
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
	//目的地の方向に回転
	if (m_Degrees >= 1.5f || m_Degrees <= -1.5f)
	{
		if (m_Degrees >= 0.0f) {
			m_Rotation.y += ToRadian(1.5f);
		}
		else {
			m_Rotation.y -= ToRadian(1.5f);
		}
	}
	else {
		//移動
		if (m_DelayCount > m_Delay) {
			if (m_TargetPos[m_AbjacentPos[m_randPos]].x > 0.1f)
				m_Position.x += m_Forward.x * m_Speed.x;
			if (m_TargetPos[m_AbjacentPos[m_randPos]].z > 0.1f)
				m_Position.z += m_Forward.z * m_Speed.z;
		}
	}
	//目的地に着いたらフラグをリセット
	if (dist < 0.2f)
	{
		//目的地の色を戻す（デバッグ用）
		if (m_DebugFlg)
			ReturnColor(m_CBall[m_AbjacentPos[m_randPos]]);
		//初期化
		m_MoveFlg = true;
		m_OneFlg = true;
		m_DelayCount = 0.0f;
	}
	//タンクとプレイヤーの間にオブジェクトがあるか判定
	Vision();	
	//間にオブジェクトがなければ中に入る
	if (!m_excFlg)
	{
		m_correctedShotTime += 1.0f / 60.0f;
		if (m_correctedShotTime >= 0.5f)
		{
			//遷移する前にフラグを初期化
			//目的地の色を戻す（デバッグ用）
			if (m_DebugFlg)
				ReturnColor(m_CBall[m_AbjacentPos[m_randPos]]);

			//初期化
			m_correctedShotTime = 0.0f;
			m_MoveFlg = true;
			m_OneFlg = true;
			m_DelayCount = 0.0f;
			//遷移フラグをtrue
			m_StateFlg = true;
		}
		
	}
	m_DelayCount += 1.0f / 60.0f;
	//情報をセット
	m_GameObject->SetPosition(m_Position);
	m_GameObject->SetRotation(m_Rotation);
	return m_StateFlg;
}
//=======================
//	内積で角度を求める
//=======================
float TankMove::face(DirectX::XMFLOAT3 targetPos)
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
//=========================================================
//		タンクとプレイヤーの間にオブジェクトがあるか判定
//=========================================================
void TankMove::Vision()
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
//==========================================
//配列の変数から2次元配列の要素を求める
//==========================================
void TankMove::ArrayPoint()
{
	int point = 0;
	//各目的地までの距離を求める
	for (int i = 0; i < TARGETPOS; i++)
	{
		if (m_Distance[i] <= 5.5f)
		{
			point = i;
			break;
		}
	}
	//一番近い座標を起点にする
	switch (point)
	{//隣接した要素を求める
	case 0:
		
		SelectAdjacent(2, 0);
		break;
	case 1:
		SelectAdjacent(2, 1);
		break;
	case 2:
		SelectAdjacent(2, 2);
		break;
	case 3:
		SelectAdjacent(2, 3);
		break;
	case 4:
		SelectAdjacent(1, 0);
		break;
	case 5:
		SelectAdjacent(1, 1);
		break;
	case 6:
		SelectAdjacent(1, 2);
		break;
	case 7:
		SelectAdjacent(1, 3);
		break;
	case 8:
		SelectAdjacent(0, 0);
		break;
	case 9:
		SelectAdjacent(0, 1);
		break;
	case 10:
		SelectAdjacent(0, 2);
		break;
	case 11:
		SelectAdjacent(0, 3);
		break;
	default:
		break;
	}
}
//=======================
//	隣接した要素を求める
//=======================
void TankMove::SelectAdjacent(int row, int col)
{
	//上方向
	if (row - 1 > -1) {
		m_AbjacentPos[0] = m_TargetPosArray[row - 1][col];
	}
	else {
		m_AbjacentPos[0] = -1;
	}
	//下方向
	if (row + 1 < ROWS) {
		m_AbjacentPos[1] = m_TargetPosArray[row + 1][col];
	}
	else {
		m_AbjacentPos[1] = -1;
	}
	//右方向
	if (col + 1 < COLS) {
		m_AbjacentPos[2] = m_TargetPosArray[row][col + 1];
	}
	else {
		m_AbjacentPos[2] = -1;
	}
	//左方向
	if (col - 1 > -1) {
		m_AbjacentPos[3] = m_TargetPosArray[row][col - 1];
	}
	else {
		m_AbjacentPos[3] = -1;
	}
}
//========================================
//	目的地の方向にオブジェクトが無いか判定
//========================================
void TankMove::MovePossibility()
{
	//変数定義
	Ray::Result result;
	Ray::Result exclusion;
	Ray::Triangle triangle[2];
	Collision::Vertices vtx;
	DirectX::XMFLOAT3 point;
	float rotY = 0.0f;
	
	//4方向分回す
	for (int i = 0; i < 4; i++)
	{
		bool checkFlg = false;

		//目的地までの距離
		DirectX::XMFLOAT3 TargetDir =	
			DirectX::XMFLOAT3(m_TargetPos[m_AbjacentPos[i]].x - m_Position.x, 
								m_TargetPos[m_AbjacentPos[i]].y - m_Position.y, 
								m_TargetPos[m_AbjacentPos[i]].z - m_Position.z);
		//ベクトルの計算
		float Dis = sqrtf(TargetDir.x * TargetDir.x + TargetDir.z * TargetDir.z);
		
		//-1の場合は除外
		if (m_AbjacentPos[i] == -1)
			continue;
		//boxの情報を取得
		std::vector<Box*> boxList = scene->GetGameObjects<Box>();
		//boxがある分回す
		for (Box* box : boxList)
		{
			//法線の数だけ回す
			for (int k = 0; k < 6; k++)
			{

				DirectX::XMFLOAT3 boxPos = box->GetPosition();
				DirectX::XMFLOAT3 boxSize = box->GetScale();
				if (k == 0)
					boxPos.x -= boxSize.x;
				if (k == 1)
					boxPos.x += boxSize.x;
				if (k == 4)
					boxPos.z -= boxSize.z;
				if (k == 5)
					boxPos.z += boxSize.z;
				if (k != 2 && k != 3)
				{

					exclusion = Ray::CheckRayPlane(m_Normal[k], boxPos, m_Position, TargetDir, Dis);
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
								//どの方向なのか判定
								switch (i)
								{
								case 0:
									m_DirObj[0] = true;
									break;
								case 1:
									m_DirObj[1] = true;
									break;
								case 2:
									m_DirObj[2] = true;
									break;
								case 3:
									m_DirObj[3] = true;
									break;
								default:
									break;
								}
								break;
							}
							//どの方向なのか判定
							switch (i)
							{
							case 0:
								m_DirObj[0] = false;
								break;
							case 1:
								m_DirObj[1] = false;
								break;
							case 2:
								m_DirObj[2] = false;
								break;
							case 3:
								m_DirObj[3] = false;
								break;
							default:
								break;

							}
						}

					}
				}
				if (checkFlg)
					break;
			}
			if (checkFlg)
				break;
		}

	}
}
