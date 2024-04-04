#include "player.h"
#include "manager.h"
#include "renderer.h"
#include "cube.h"
#include "shader.h"
#include "input.h"
#include "modelRenderer.h"
#include "pbullet.h"
#include "box.h"
#include "explosion.h"
#include "audio.h"
#include "reflection.h"
#include "alongthewall.h"

//ImGui
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"


//===================================
//				初期化
//===================================
void Player::Init()
{
	//----------コンポーネント取得--------------
	AddComponent<Shader>()->Load("shader/vertexLightingVS.cso", "shader/vertexLightingPS.cso");//シェーダ
	AddComponent<ModelRenderer>()->Load("asset/Model/PTank.obj");							   //モデル
	AddComponent<AlongTheWall>()->SetPlayer(this);											   //当たり判定
	m_SE = AddComponent<Audio>();															   //SE
	//-------------SE取得-----------------
	m_SE->Load("asset/audio/BulletSound.wav");												   

	//--------------初期化-------------
	SetPosition(DirectX::XMFLOAT3(0.0f, 0.5f, 0.0f));	//プレイヤーのサイズを設定
	m_Delay = 1.5f;										//再度弾を撃てるようにする時間を設定
	m_DelayCount = 1.5f;								//プレイ直後に弾を撃てるように設定
	m_Speed = { 0.1f,0.1f,0.1f };						//移動速度を設定
}
//===================================
//				更新
//===================================
void Player::Update()
{
	//シーンの情報を取得
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();

	//カウント
	m_DelayCount += 1.0f / 60.0f;

	//プレイヤーがいなければ以降の処理を行わない
	if (player == NULL)
		return;
	//プレイヤーの向いている方向を取得
	DirectX::XMFLOAT3 forward = GetForward();
	//"A"を押し続けている間左回転させる
	if (Input::GetKeyPress('A'))
	{
		m_Rotation.y -= 0.05f;
	}
	//"D"を押し続けている間右回転させる
	if (Input::GetKeyPress('D'))
	{
		m_Rotation.y += 0.05f;
	}
	//"W"を押し続けている間前に進む
	if (Input::GetKeyPress('W'))
	{
		m_Position.x += forward.x * m_Speed.x;
		m_Position.z += forward.z * m_Speed.z;
	}
	//"S"を押し続けている間後ろに進む
	if (Input::GetKeyPress('S'))
	{
		m_Position.x -= forward.x * m_Speed.x;
		m_Position.z -= forward.z * m_Speed.z;
	}
	
	//重力
	m_Velocity.y -= 0.015f;

	//抵抗
	m_Velocity.y -= m_Velocity.y * 0.01f;

	//移動
	m_Position.x += m_Velocity.x;
	m_Position.z += m_Velocity.z;

	// 弾発射
	//弾を撃ったら間隔を空ける
	if (m_DelayCount >= m_Delay)
	{
		//"K"を押したら弾を撃つ
		if (Input::GetKeyTrigger('K'))
		{
			//弾の情報を取得
			PBullet* bullet = scene->AddGameObject<PBullet>();
			//初期座標をセット
			bullet->SetPosition(DirectX::XMFLOAT3(m_Position.x + 0.0f,
				m_Position.y + 1.0f, m_Position.z + 0.0f));
			//スピードを加える
			bullet->SetVelocity(DirectX::XMFLOAT3(forward.x * 0.15f,
				forward.y * 0.15f, forward.z * 0.15f));
			
			//SE
			m_SE->Play();
			//カウントをリセットする
			m_DelayCount = 0.0f;
		}
	}
	//デバッグがオンの時に表示
	if (m_DebugFlg)
	{
		ImGui::Begin("PlayerPos");
		ImGui::Text("PlayerPos: x:%.2f,y:%.2f,z:%.2f", m_Position.x, m_Position.y, m_Position.z);
		ImGui::End();
	}
}





