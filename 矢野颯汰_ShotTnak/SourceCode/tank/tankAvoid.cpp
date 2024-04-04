#include "tankAvoid.h"
#include "box.h"
#include "ray.h"
#include "collision.h"
#include "pbullet.h"
#include "CreateLine.h"
#include "manager.h"
#include "XMFLOAT_Helper.h"
//=======================
//		初期化
//=======================
void TankAvoid::Init()
{
	scene = Manager::GetScene();	//シーン
	m_avertDelay = 2.0f;
	m_avertCount = 10.0f;
	m_MoveTime = 3.0f;
	m_HitBullet.hit = false;
	m_AvoidFlg = false;
	
}
//=======================
//		逃げる
//=======================
bool TankAvoid::Avoid()
{
	m_Position = m_GameObject->GetPosition();//座標
	m_Rotation = m_GameObject->GetRotation();//角度
	m_Forward = m_GameObject->GetForward();	 //前方ベクトル
	m_Speed = m_GameObject->GetSpeed();		 //速度
	m_CollisionFlg = m_GameObject->GetCollisionFlg();	//当たり判定フラグ
	bool stateFlg = false;

	//弾があたりそうか判定
	Possibility();
	//trueの時逃げない
	if (!m_AvoidFlg)
	{
		//当たりそうなとき中に入る
		if (m_HitBullet.hit)
		{
			//4方向
			for (int i = 0; i < 4; i++)
			{
				//逃げる方向
				//０度、９０度、－９０度、１８０度
				switch (i)
				{
				case 0:
					m_RayDir[i] = m_Forward;
					break;
				case 1:
					m_RayDir[i] = { m_Forward.z,0.0f,-m_Forward.x };
					break;
				case 2:
					m_RayDir[i] = { -m_Forward.z,0.0f,m_Forward.x };
					break;
				case 3:
					m_RayDir[i] = { -m_Forward.x,0.0f,-m_Forward.z };
					break;

				default:
					break;
				}
			}
			//boxの情報を取得
			std::vector<Box*> boxList = scene->GetGameObjects<Box>();
			//boxの数回す
			for (Box* box : boxList)
			{
				bool Endflg = false;//trueになった時処理をぬける
				//法線の数だけ回す
				for (int i = 0; i < 4; i++)
				{
					//レイを飛ばす方向の数だけ回す
					for (int j = 0; j < 4; j++)
					{
						DirectX::XMFLOAT3 boxPos = box->GetPosition();//座標
						DirectX::XMFLOAT3 boxSize = box->GetScale();  //サイズ
						//法線の方向のサイズを加える
						switch (i)
						{
						case 0:
							boxPos.x -= boxSize.x;
							break;
						case 1:
							boxPos.x += boxSize.x;
							break;
						case 2:
							boxPos.z -= boxSize.z;
							break;
						case 3:
							boxPos.z += boxSize.z;
							break;
						default:
							break;
						}

						//上下左右にレイを飛ばす
						Ray::Result result = Ray::CheckRayPlane(m_Dir4[i],
																boxPos, 
																m_Position, 
																m_RayDir[j], 
																30.0f);
						//Rayがboxに当たった時中に入る
						if (result.hit)
						{
							//レイが当たった座標を格納
							m_FacePoint[j] = result.point;
							Endflg = true;	//処理をぬける
							if (Endflg)
								break;
						}
						if (Endflg)
							break;
					}
					if (Endflg)
						break;
				}
				if (Endflg)
					continue;
			}
			//レイが当たった面までの距離を計算し、ソート
			LenSort();
			m_AvoidFlg = true;
			//内積の判定で左右
			m_tempAngle = PointAngle();
			m_PAngle = 0.0f;		//初期化
			m_MoveCount = 0.0f;		//初期化
			DebugOneFlg = DebugFlg;	//初期化
		}
	}
	//Countの方が大きければ中に入る
	if (m_avertCount >= m_avertDelay)
	{
		//デバッグ用のline
		CreateLine* line;
		line = scene->GetGameObject<CreateLine>();

		//角度を補正
		if (m_PAngle > 6.28f)
			m_PAngle -= 6.28f;
		if (m_PAngle < 0.0f)
			m_PAngle += 6.28f;

		//逃げる方向に向いていれば中に入る
		if (m_PAngle <= m_tempAngle + 0.05f && m_PAngle >= m_tempAngle - 0.05f)
		{
			//移動先を計算（デバック表示用
			if (DebugFlg)
			{
				DebugPos = m_Position;
				DebugTime = m_MoveCount;
				while (DebugOneFlg)
				{
					DebugPos.x += m_Forward.x * m_Speed.x;
					DebugPos.z += m_Forward.z * m_Speed.z;

					DebugTime += 1.0f / 60.0f;
					if (DebugTime >= m_MoveTime)
					{
						line = scene->AddGameObject<CreateLine>();
						line->PosCreate(DebugPos, m_Position);
						DebugOneFlg = false;
					}
				}
				line->PosCreate(DebugPos, m_Position);
			}
			//移動
			m_Position.x += m_Forward.x * m_Speed.x;
			m_Position.z += m_Forward.z * m_Speed.z;
		}
		else
		{
			//逃げる方向に回転
			if (m_tempAngle <= 3.14)
			{
				m_Rotation.y += 0.05f;
				m_PAngle += 0.05f;
			}
			else
			{
				m_Rotation.y -= 0.05f;
				m_PAngle -= 0.05f;
			}
		}
		//一定時間逃げる
		if (m_MoveCount >= m_MoveTime || m_CollisionFlg){
			//初期化
			m_AvoidFlg = false;
			m_avertCount = 0.0f;
			stateFlg = true;
			//デバッグ用lineを削除
			if (line != nullptr)
				line->SetDestroy();
		}
		//逃げる時間のカウント
		m_MoveCount += 1.0f / 60.0f;

	}
	else if (m_AvoidFlg &&m_avertCount <= m_avertDelay)
		stateFlg = true;	//ステートを遷移
	//情報を渡す
	m_GameObject->SetPosition(m_Position);
	m_GameObject->SetRotation(m_Rotation);

	return stateFlg;
}
//=======================
//			昇順ソート
//=======================
void TankAvoid::LenSort()
{
	//初期化
	DirectX::XMFLOAT3 point[4] = { {0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	
	for (int i = 0; i < 4; i++)
	{
		//ポイントまでのベクトル
		point[i].x = m_Position.x - m_FacePoint[i].x;
		point[i].z = m_Position.z - m_FacePoint[i].z;

		//絶対値に変換
		point[i].x = fabsf(point[i].x);
		point[i].z = fabsf(point[i].z);
		//移動ベクトルの計算
		m_Vec[i] = sqrt(point[i].x * point[i].x +
			point[i].z * point[i].z);
	}
	//ソート
	for (int i = 0; i < 4; i++) {
		for (int j = i; j < 4; j++) {
			if (m_Vec[i] > m_Vec[j]) {
				//値の交換
				float temp = m_Vec[i];
				m_Vec[i] = m_Vec[j];
				m_Vec[j] = temp;

				//当たった座標もソート
				DirectX::XMFLOAT3 temp3 = m_FacePoint[i];
				m_FacePoint[i] = m_FacePoint[j];
				m_FacePoint[j] = temp3;

			}
		}
	}
}
//================================
//		外積を使った角度の計算
//================================
float TankAvoid::PointAngle()
{
	//変数定義
	float Rot;
	DirectX::XMFLOAT3 tankDir;
	DirectX::XMFLOAT3 RotateDir;

	//正規化
	DirectX::XMVECTOR DirVector = DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&m_FacePoint[3]));

	DirectX::XMStoreFloat3(&tankDir, DirVector);
	//内積の計算
	DirectX::XMStoreFloat(&Rot, DirectX::XMVector3Dot(
		DirectX::XMVector3Normalize(DirVector),
		DirectX::XMLoadFloat3(&m_Forward)));
	Rot = ::acos(Rot);
	//外積の計算
	DirectX::XMStoreFloat3(&RotateDir,
		DirectX::XMVector3Cross(
			DirectX::XMVector3Normalize(DirVector),
			DirectX::XMLoadFloat3(&m_Forward)));
	//角度の補正
	if (RotateDir.y > 0) {
		Rot = 6.28 - Rot;
	}
	return Rot;
}
//===================================
//		敵が弾を視認できているか判定
//===================================
void TankAvoid::Possibility()
{
	//処理をする前に初期化
	m_HitBullet.dot = 0.0f;
	m_HitBullet.hit = false;
	m_HitBullet.pos = { 0.0f,0.0f,0.0f };
	m_HitBullet.velo = { 0.0f,0.0f,0.0f };
	//変数定義
	Ray::Result result;
	Ray::Result exclusion;
	Ray::Triangle triangle[2];
	Collision::Vertices vtx;
	DirectX::XMFLOAT3 point;
	float rotY = 0.0f;
	bool checkFlg = false;
	//弾の情報をシーン内から取得
	pbullet = scene->GetGameObjects<PBullet>();
	//シーン内にある弾の数回す
	for (PBullet* bullet : pbullet)
	{
		DirectX::XMFLOAT3 bulletVec = bullet->GetVelocity();	//弾のベクトル
		DirectX::XMFLOAT3 bulletPos = bullet->GetPosition();	//弾の座標
		DirectX::XMFLOAT3 TargetDir = 							//弾とタンクの距離
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
						//レイの計算
						exclusion = Ray::CheckRayPlane(m_Normal[k], boxPos, m_Position, TargetDir, Dis + 0.1f);
						if (exclusion.hit)
						{
							//頂点情報を取得
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
			//弾が当たりそうなら中に入る
			if (!checkFlg)
			{
				//弾とタンクの角度
				rotY = face(bulletPos);
				//弾を感知する角度
				if (rotY <= 0.3f && rotY >= -0.3f)
				{
					//当たりそうか内積を使って計算
					m_HitBullet = Dot(bulletVec, bulletPos);
					break;
				}
			}
		}
	}
}
//===================================
//		内積で角度を求める
//===================================
float TankAvoid::face(DirectX::XMFLOAT3 targetPos)
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
//===================================
//		内積の結果をHitBulletに格納
//===================================
TankAvoid::HitBullet TankAvoid::Dot(DirectX::XMFLOAT3 vec, DirectX::XMFLOAT3 pos)
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
		hit.pos = { 0.0f,0.0f,0.0f };
		hit.velo = { 0.0f,0.0f,0.0f };
		hit.dot = 0.0f;
	}
	return hit;
}
