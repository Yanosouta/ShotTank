// Bulletオブジェクト [bullet.cpp]
#include "bullet.h"
#include "modelRenderer.h"
#include "shader.h"
#include "tank.h"
#include "explosion.h"
#include "reflection.h"

using DirectX::operator*;
//=========================
//			初期化
//=========================
void Bullet::Init()
{
	//コンポーネントの追加
	AddComponent<Shader>()->Load(											//シェーダ
		"shader/vertexLightingVS.cso", "shader/vertexLightingPS.cso");		
	AddComponent<ModelRenderer>()->Load("asset/model/Debugbullet.obj");		//モデル
	AddComponent<Reflection>()->SetBullet(this);							//反射
}
//=========================
//			更新
//=========================
void Bullet::Update()
{
	//シーンの情報を取得
	scene = Manager::GetScene();
	//プレイヤーの情報を取得
	player = scene->GetGameObject<Player>();
	//プレイヤーがいなければ以降の処理を行わない
	if (player == NULL)
		return;
	//弾の移動
	m_Position.x += m_Velocity.x;
	m_Position.y += m_Velocity.y;
	m_Position.z += m_Velocity.z;
	
	//当たり判定
	collision();
}
//=========================
//		当たり判定
//=========================
void Bullet::collision()
{	
	//座標の取得
	DirectX::XMFLOAT3 position = player->GetPosition();
	//ベクトル
	DirectX::XMFLOAT3 direction;
	//ベクトルの計算
	direction.x = position.x - m_Position.x;
	direction.y = position.y - m_Position.y;
	direction.z = position.z - m_Position.z;
	//距離の計算
	DirectX::XMStoreFloat(&m_length,
		DirectX::XMVector3LengthSq(DirectX::XMLoadFloat3(&direction)));
	//当たっているか判定
	if (m_length < (2.0f * 2.0f))
	{
		//エフェクト
		Explosion* explosion = scene->AddGameObject<Explosion>();
		
		explosion->SetScale(DirectX::XMFLOAT3(1.5f, 1.5f, 1.0f));	//サイズをセット
		explosion->SetPosition(DirectX::XMFLOAT3(position.x,		//座標をセット
			position.y + 1.0f, position.z));

		//削除
		player->SetDestroy();	//プレイヤー
		SetDestroy();			//弾の削除
		return;
	}

}


