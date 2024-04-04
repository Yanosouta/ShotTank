#include "alongthewall.h"

void AlongTheWall::Init()
{
	m_Scene = Manager::GetScene();
	m_Box = m_Scene->GetGameObjects<Box>();
}

void AlongTheWall::Update()
{
	m_Pos = m_GameObject->GetPosition();
	m_Scale = m_GameObject->GetScale();
	m_Velo = m_GameObject->GetForward();
	m_collisionFlg = false;
	m_Speed = { 0.1f,0.1f, 0.1f };
	//進行ベクトルの計算
	
	for (int i = 0; i < m_Box.size(); i++)
	{
		m_BoxPos = m_Box[i]->GetPosition();
		m_BoxScale = m_Box[i]->GetScale();
		if (UpCollision(m_BoxPos, m_BoxScale) &&
			DownCollision(m_BoxPos, m_BoxScale) &&
			LeftCollision(m_BoxPos, m_BoxScale) &&
			RightCollision(m_BoxPos, m_BoxScale))
		{
			m_collisionFlg = true;
			//右壁との判定
			if (m_Pos.x < m_BoxPos.x  && m_Pos.z >= (m_BoxPos.z - m_BoxScale.z) && m_Pos.z <= (m_BoxPos.z + m_BoxScale.z))//上
			{
				m_TouchNor = m_Normal[1];
				//Boxの法線とプレイヤーのベクトルが平行か判定(平行なほど0に近い)
				DirectX::XMVECTOR Cross = DirectX::XMVector3Cross(DirectX::XMLoadFloat3(&m_TouchNor), DirectX::XMLoadFloat3(&m_Velo));
				DirectX::XMStoreFloat3(&m_fcross, Cross);
				if (m_fcross.y > -1.0f && m_fcross.y < 1.0f)	
				{
					m_Pos.x = m_BoxPos.x - m_BoxScale.x + -m_Scale.x + 0.01f;
					m_Speed.z = 0.08f;
					
				}
			}	
			//左壁のと判定
			else if (m_Pos.x > m_BoxPos.x  && m_Pos.z >= (m_BoxPos.z - m_BoxScale.z) && m_Pos.z <= (m_BoxPos.z + m_BoxScale.z))//下
			{
				m_TouchNor = m_Normal[0];
				DirectX::XMVECTOR Cross = DirectX::XMVector3Cross(DirectX::XMLoadFloat3(&m_TouchNor), DirectX::XMLoadFloat3(&m_Velo));
				DirectX::XMStoreFloat3(&m_fcross, Cross);
				if (m_fcross.y > -1.0f && m_fcross.y < 1.0f)
				{
					m_Pos.x = m_BoxPos.x + m_BoxScale.x - -m_Scale.x  + 0.01f ;
					m_Speed.z = 0.08f;
				}
			}
			//上壁との判定
			else if (m_Pos.z < m_BoxPos.z  && m_Pos.x >= (m_BoxPos.x - m_BoxScale.x) && m_Pos.x <= (m_BoxPos.x + m_BoxScale.x))
			{
				m_TouchNor = m_Normal[4];
				DirectX::XMVECTOR Cross = DirectX::XMVector3Cross(DirectX::XMLoadFloat3(&m_TouchNor), DirectX::XMLoadFloat3(&m_Velo));
				DirectX::XMStoreFloat3(&m_fcross, Cross);
				if (m_fcross.y > -1.0f && m_fcross.y < 1.0f)
				{
					m_Pos.z = m_BoxPos.z - m_BoxScale.z + -m_Scale.z + 0.01f;
					
					m_Speed.x = 0.08f;
				}
			}
			//下壁との判定
			else if (m_Pos.z > m_BoxPos.z  && m_Pos.x >= (m_BoxPos.x - m_BoxScale.x) && m_Pos.x <= (m_BoxPos.x + m_BoxScale.x))
			{
				m_TouchNor = m_Normal[5];
				DirectX::XMVECTOR Cross = DirectX::XMVector3Cross(DirectX::XMLoadFloat3(&m_TouchNor), DirectX::XMLoadFloat3(&m_Velo));
				DirectX::XMStoreFloat3(&m_fcross, Cross);
				if (m_fcross.y > -1.0f && m_fcross.y < 1.0f)
				{
					m_Pos.z = m_BoxPos.z + m_BoxScale.z - -m_Scale.z - 0.01f;
					m_Speed.x = 0.08f;
				}
			}
		}
		m_GameObject->SetSpeed(m_Speed);
		m_GameObject->SetPosition(m_Pos);
		m_GameObject->SetCollisionFlg(m_collisionFlg);
		
	}
	
}

void AlongTheWall::SetPlayer(Player * player)
{
	m_Player = player;
}

void AlongTheWall::SetTank(Tank *tank)
{
	m_Tank = tank;
}

bool AlongTheWall::UpCollision(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 scale)
{
	return pos.x - scale.x - 1.0f < m_Pos.x;
}

bool AlongTheWall::DownCollision(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 scale)
{
	return m_Pos.x < pos.x + scale.x + 1.0f;
}

bool AlongTheWall::LeftCollision(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 scale)
{
	return pos.z - scale.z - 1.0f < m_Pos.z;
}

bool AlongTheWall::RightCollision(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 scale)
{
	return m_Pos.z < pos.z + scale.z + 1.0f;
}
