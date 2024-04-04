#include "reflection.h"
#include "manager.h"
//==========================
//			初期化
//==========================
void Reflection::Init()
{
	//シーンの情報を取得
	m_Scene = Manager::GetScene();
	//シーン内のブロックの情報を取得
	m_Box = m_Scene->GetGameObjects<Box>();
	//反射の回数
	m_Refl = 1;
	//反射の数測定
	m_ReflCount = 0;
}
//===========================
//			更新
//===========================
void Reflection::Update()
{
	//弾がなければ以降の処理を行わない
	if (m_Bullet == NULL)
		return;
	//弾の情報を取得
	m_BulletPos = m_GameObject->GetPosition();
	m_BulletScale = m_GameObject->GetScale();
	m_BulletVel = m_GameObject->GetVelocity();
	//boxの数だけ回す
	for (int i = 0; i < m_Box.size(); i++)
	{
		//ブロックごとの情報を取得
		m_BoxPos = m_Box[i]->GetPosition();
		m_BoxScale = m_Box[i]->GetScale();

		//当たり判定
		if (UpCollision(m_BoxPos, m_BoxScale) &&
			DownCollision(m_BoxPos, m_BoxScale) &&
			LeftCollision(m_BoxPos, m_BoxScale) &&
			RightCollision(m_BoxPos, m_BoxScale))
		{
			//どの面に当たったか判定
			if (m_BulletPos.x < m_BoxPos.x  && m_BulletPos.z >= (m_BoxPos.z - m_BoxScale.z) && m_BulletPos.z <= (m_BoxPos.z + m_BoxScale.z))//上
				m_TouchNor = m_Normal[1];
			else if (m_BulletPos.x > m_BoxPos.x  && m_BulletPos.z >= (m_BoxPos.z - m_BoxScale.z) && m_BulletPos.z <= (m_BoxPos.z + m_BoxScale.z))//下
				m_TouchNor = m_Normal[0];
			else if (m_BulletPos.z < m_BoxPos.z  && m_BulletPos.x >= (m_BoxPos.x - m_BoxScale.x) && m_BulletPos.x <= (m_BoxPos.x + m_BoxScale.x))//左
				m_TouchNor = m_Normal[4];
			else if (m_BulletPos.z > m_BoxPos.z  && m_BulletPos.x >= (m_BoxPos.x - m_BoxScale.x) && m_BulletPos.x <= (m_BoxPos.x + m_BoxScale.x))//右
				m_TouchNor = m_Normal[5];

			//反射ベクトルの計算
			m_BulletVel.x = m_BulletVel.x + 2.0f*(-m_BulletVel.x * m_TouchNor.x) * m_TouchNor.x;
			m_BulletVel.z = m_BulletVel.z + 2.0f*(-m_BulletVel.z * m_TouchNor.z) * m_TouchNor.z;
			//値を返す
			m_GameObject->SetVelocity(m_BulletVel);
		
			m_ReflCount++;
			
		}
	}
	//規定以上の回数反射したら削除
	if(m_ReflCount > m_Refl)
		m_GameObject->SetDestroy();
}
//==============================
//		上の当たり判定
//==============================
bool Reflection::UpCollision(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 scale)
{
	return pos.x - scale.x - 0.5f < m_BulletPos.x;
}
//==============================
//		下の当たり判定
//==============================
bool Reflection::DownCollision(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 scale)
{
	return m_BulletPos.x < pos.x + scale.x + 0.5f;
}
//==============================
//		左の当たり判定
//==============================
bool Reflection::LeftCollision(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 scale)
{
	return pos.z - scale.z - 0.5f < m_BulletPos.z;
}
//==============================
//		右の当たり判定
//==============================
bool Reflection::RightCollision(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 scale)
{
	return m_BulletPos.z < pos.z + scale.z + 0.5f;
}
//==============================
//		敵の弾の情報を取得
//==============================
void Reflection::SetBullet(Bullet* bullet)
{
	m_Bullet = bullet;
}
//==============================
//		敵の弾の情報を取得
//==============================
void Reflection::SetPBullet(PBullet* pbullet)
{
	m_PBullet = pbullet;
}
