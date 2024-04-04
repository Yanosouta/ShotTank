// Bulletオブジェクト [bullet.cpp]
#include "pbullet.h"
#include "modelRenderer.h"
#include "shader.h"
#include "explosion.h"
#include "player.h"
#include "reflection.h"
#include "tank.h"
using DirectX::operator*;

//=====================
//		初期化
//=====================
void PBullet::Init()
{
	//コンポーネントの追加
	AddComponent<Shader>()->Load(										//シェーダ
		"shader/vertexLightingVS.cso", "shader/vertexLightingPS.cso");
	AddComponent<ModelRenderer>()->Load("asset/model/Pbullet.obj");		//モデル
	AddComponent<Reflection>()->SetPBullet(this);						//反射
}
//=====================
//		　更新
//=====================
void PBullet::Update()
{
	//シーンの情報を取得
	scene = Manager::GetScene();
	//タンクの情報を取得
	std::vector<Tank*> tankList = scene->GetGameObjects<Tank>();

	//弾の移動
	m_Position.x += m_Velocity.x;
	m_Position.y += m_Velocity.y;
	m_Position.z += m_Velocity.z;
	//タンクがいなければ以降の処理を行わない
	if (tankList.size() == NULL)
		return;

	//当たり判定の計算
	//タンクの数回す
	for (Tank* tank : tankList)
	{
		//座標の取得
		DirectX::XMFLOAT3 position = tank->GetPosition();
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
			explosion->SetScale(DirectX::XMFLOAT3(1.5f, 1.5f, 1.0f));//サイズ
			explosion->SetPosition(DirectX::XMFLOAT3(position.x,	 //座標
				position.y + 1.0f, position.z));

			//削除
			tank->SetDestroy();	//タンクの削除
			SetDestroy();		//弾の削除
			return;
		}
	}
	
}

//=============================
//	  弾の色の変更(デバッグ用)
//=============================
void PBullet::ChangeColor()
{
	GetComponent<ModelRenderer>()->Load("asset/model/Debugbullet.obj");
}

