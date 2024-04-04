#include "tankShot.h"
#include "bullet.h"

#include "ray.h"
#include "collision.h"
void TankShot::Init()
{
	scene = Manager::GetScene();
	player = scene->GetGameObject<Player>();
	m_FrameCount = 0.0f;
	m_RotY = 0.0f;
	m_BulletTime = 1.3f;
}

bool TankShot::Shot()
{
	bool stateFlg = false;
	float correctedRot = 0.0f;
	DirectX::XMFLOAT3 playerPos = player->GetPosition();

	m_Position = m_GameObject->GetPosition();
	m_Rotation = m_GameObject->GetRotation();
	m_Forward = m_GameObject->GetForward();

	m_FrameCount++;
	//フレームが５フレーム以上たっていたらプレイヤーの方向に向き始める
	if (m_FrameCount > 5)
	{
		m_RotY = face(playerPos);
		correctedRot = Pface(playerPos);
	}
	else
	{
		stateFlg = true;
		return stateFlg;
	}
	//撃ってから0.5秒後以上かつ５フレームたっていたら
	if (m_BulletTime > 1.5f && m_FrameCount > 5)
	{
		m_BulletTime = 0.0f;
		Bullet* bullet = scene->AddGameObject<Bullet>();
		bullet->SetPosition(DirectX::XMFLOAT3(m_Position.x + 0.0f,
			m_Position.y + 1.5f, m_Position.z + 0.0f));
		bullet->SetVelocity(DirectX::XMFLOAT3(m_Forward.x * 0.15f,
			m_Forward.y, m_Forward.z * 0.15f));
	}

	//内積の判定で左右
	if ((m_RotY >= 0.05f || m_RotY <= -0.05f )|| correctedRot <= -0.95)
	{
		if (m_RotY >= 0.0f)
		{
			m_Rotation.y += 0.05f;
		}
		else
		{
			m_Rotation.y -= 0.05f;
		}
	}
	//プレイヤーの方向を向いていたらカウントを進める
	if (m_RotY <= 0.2f && m_RotY >= -0.2f)
	{

		m_BulletTime += 1.0f / 60.0f;
	}

	//プレイヤーを見つけたら遷移
	Vision();		//タンクとプレイヤーの間にオブジェクトがあるか判定
	if (!m_excFlg)
	{
		stateFlg = true;
	}
	m_GameObject->SetRotation(m_Rotation);
	return stateFlg;
}

float TankShot::face(DirectX::XMFLOAT3 targetPos)
{
	DirectX::XMFLOAT3 TargetDir;
	DirectX::XMVECTOR DirVector;
	float rotY = 0.0f;
	DirectX::XMFLOAT3 forward = m_Forward;
	//位置ベクトルの計算
	TargetDir = DirectX::XMFLOAT3(targetPos.x - m_Position.x, targetPos.y - m_Position.y, targetPos.z - m_Position.z);
	//正規化
	DirVector = DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&TargetDir));
	//内積の計算
	//90°補正
	DirectX::XMMATRIX  rotMat = DirectX::XMMatrixRotationY(3.14f / 2);
	DirectX::XMStoreFloat3(&forward, DirectX::XMVector3Normalize(DirectX::XMVector3Transform(DirectX::XMLoadFloat3(&forward), rotMat)));

	DirectX::XMStoreFloat(&rotY, DirectX::XMVector3Dot(
		DirectX::XMVector3Normalize(DirVector),
		DirectX::XMLoadFloat3(&forward)));

	return rotY;
}

float TankShot::Pface(DirectX::XMFLOAT3 targetPos)
{
	DirectX::XMFLOAT3 TargetDir;
	DirectX::XMVECTOR DirVector;
	float rotY = 0.0f;
	DirectX::XMFLOAT3 forward = m_Forward;
	//位置ベクトルの計算
	TargetDir = DirectX::XMFLOAT3(targetPos.x - m_Position.x, targetPos.y - m_Position.y, targetPos.z - m_Position.z);
	//正規化
	DirVector = DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&TargetDir));
	DirectX::XMStoreFloat(&rotY, DirectX::XMVector3Dot(
		DirectX::XMVector3Normalize(DirVector),
		DirectX::XMLoadFloat3(&forward)));

	return rotY;
}

void TankShot::Vision()
{
	Ray::Result result;
	Ray::Result exclusion;
	Ray::Triangle triangle[2];
	Collision::Vertices vtx;
	DirectX::XMFLOAT3 point;
	m_excFlg = false;

	DirectX::XMFLOAT3  playerPos = player->GetPosition();
	DirectX::XMFLOAT3 TargetDir = DirectX::XMFLOAT3(playerPos.x - m_Position.x, playerPos.y - m_Position.y, playerPos.z - m_Position.z);

	float Dis = sqrtf(TargetDir.x * TargetDir.x + TargetDir.z * TargetDir.z);
	//プレイヤーまでレイを飛ばす
	for (int i = 0; i < 6; i++)
	{
		if (i != 2 && i != 3)
		{

			std::vector<Box*> boxList = scene->GetGameObjects<Box>();

			for (Box* box : boxList)
			{

				vtx = box->GetComponent<Collision>()->GetVertices();
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
									m_excFlg = true;
									m_FrameCount = 0;
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
