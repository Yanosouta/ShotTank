
#include "manager.h"
#include "renderer.h"

#include "camera.h"
#include "field.h"
#include "box.h"
#include "player.h"
#include "sky.h"
#include "game.h"
#include "bullet.h"
#include "Pbullet.h"
#include "explosion.h"
#include "result.h"
#include "title.h"
#include "transition.h"
#include "input.h"
#include "audio.h"
#include "tank.h"
#include "collision.h"
#include "CreateLine.h"
#include "wall.h"

//test
#include "AStar.h"
//imgui
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"


#define WALL_X 20		//�ǂ̃e�N�X�`���̐�
#define WALL_Z 11		//�ǂ̃e�N�X�`���̐�
#define MAX_WALL 100	//�ǂ̃e�N�X�`���̍ő吔
//==============================
//				������
//==============================
void Game::Init()
{
	//������
	int upPos, leftPos, rightPos, downPos;
	int num = 0;
	upPos = 0;
	leftPos = 0;
	rightPos = 0;
	downPos = 0;
	Explosion::Load();

	AddGameObject<Camera>();	//�J����
	AddGameObject<Sky>();		//�w�i
	//��
	{
		Field* field = AddGameObject<Field>();
		field->SetPosition(DirectX::XMFLOAT3(19.0f, 0.0f, 10.0f));
		field->SetScale(DirectX::XMFLOAT3(10.0f, 1.5f, 10.0f));
	}
	//���
	{
		Box* box = AddGameObject<Box>();
		box->SetPosition(DirectX::XMFLOAT3(18.0f, 0.0f, 22.0f));
		box->SetScale(DirectX::XMFLOAT3(20.0f, 2.0f, 2.0f));
		Collision::Vertices vtx = box->AddComponent<Collision>()->GetVertices();
	}
	//�E��
	{
		Box* box = AddGameObject<Box>();
		box->SetPosition(DirectX::XMFLOAT3(38.0f, 0.0f, 9.0f));
		box->SetScale(DirectX::XMFLOAT3(2.0f, 2.0f, 13.0f));
		Collision::Vertices vtx = box->AddComponent<Collision>()->GetVertices();
	}
	//����
	{
		Box* box = AddGameObject<Box>();
		box->SetPosition(DirectX::XMFLOAT3(-2.0f, 0.0f, 9.0f));
		box->SetScale(DirectX::XMFLOAT3(2.0f, 2.0f, 13.0f));
		Collision::Vertices vtx = box->AddComponent<Collision>()->GetVertices();
	}
	//����
	{
		Box* box = AddGameObject<Box>();
		box->SetPosition(DirectX::XMFLOAT3(18.0f, 0.0f, -2.0f));
		box->SetScale(DirectX::XMFLOAT3(20.0f, 2.0f, 2.0f));
		Collision::Vertices vtx = box->AddComponent<Collision>()->GetVertices();
	}
	//�E
	{
		Box* box = AddGameObject<Box>();
		box->SetPosition(DirectX::XMFLOAT3(30.0f, 0.0f, 10.0f));
		box->SetScale(DirectX::XMFLOAT3(1.0f, 2.0f, 5.0f));
		Collision::Vertices vtx = box->AddComponent<Collision>()->GetVertices();
	}
	//�^��
	{
		Box* box = AddGameObject<Box>();
		box->SetPosition(DirectX::XMFLOAT3(6.0f, 0.0f, 10.0f));
		box->SetScale(DirectX::XMFLOAT3(1.0f, 2.0f, 5.0f));
		Collision::Vertices vtx = box->AddComponent<Collision>()->GetVertices();
	}
	//��
	{
		Box* box = AddGameObject<Box>();
		box->SetPosition(DirectX::XMFLOAT3(18.0f, 0.0f, 10.0f));
		box->SetScale(DirectX::XMFLOAT3(1.0f, 2.0f, 5.0f));
		Collision::Vertices vtx = box->AddComponent<Collision>()->GetVertices();
	}

	//�e�N�X�`��
	Wall* wall[MAX_WALL];
	//���
	for (int i = 0; i < WALL_X; i++)
	{
		upPos += 2;
		wall[i] = AddGameObject<Wall>();
		wall[i]->SetPosition(DirectX::XMFLOAT3(upPos - 1, 1.0f, 20.9f));
		wall[i]->SetScale(DirectX::XMFLOAT3(1.0f, 1.2f, 1.0f));
	}
	//�E��
	for (int i = 0; i < WALL_Z; i++)
	{
		leftPos += 2;
		wall[i] = AddGameObject<Wall>();
		wall[i]->SetPosition(DirectX::XMFLOAT3(-0.9f, 1.0f, leftPos - 1));
		wall[i]->SetScale(DirectX::XMFLOAT3(1.0f, 1.2f, 1.0f));
	}
	//����
	for (int i = 0; i < WALL_Z; i++)
	{
		rightPos += 2;
		wall[i] = AddGameObject<Wall>();
		wall[i]->SetPosition(DirectX::XMFLOAT3(36.9f, 1.0f, rightPos - 1));
		wall[i]->SetScale(DirectX::XMFLOAT3(1.0f, 1.2f, 1.0f));
	}
	//����
	for (int i = 0; i < WALL_X; i++)
	{
		downPos += 2;
		wall[i] = AddGameObject<Wall>();
		wall[i]->SetPosition(DirectX::XMFLOAT3(downPos - 3, 1.0f, -0.9f));
		wall[i]->SetScale(DirectX::XMFLOAT3(1.0f, 1.2f, 1.1f));
	}
	//�E
	for (int i = 0; i < 5; i++)
	{
		num += 2;
		wall[i] = AddGameObject<Wall>();
		wall[i]->SetPosition(DirectX::XMFLOAT3(30.0f, 1.0f, num + 4));
		wall[i]->SetScale(DirectX::XMFLOAT3(1.1f, 1.2f, 1.1f));
	}
	num = 0;
	//�^��
	for (int i = 0; i < 5; i++)
	{
		num += 2;
		wall[i] = AddGameObject<Wall>();
		wall[i]->SetPosition(DirectX::XMFLOAT3(6.0f, 1.0f, num + 4));
		wall[i]->SetScale(DirectX::XMFLOAT3(1.1f, 1.2f, 1.1f));
	}
	num = 0;
	//��
	for (int i = 0; i < 5; i++)
	{
		num += 2;
		wall[i] = AddGameObject<Wall>();
		wall[i]->SetPosition(DirectX::XMFLOAT3(18.0f, 1.0f, num + 4));
		wall[i]->SetScale(DirectX::XMFLOAT3(1.1f, 1.2f, 1.1f));
	}
	
	//�v���C���[
	AddGameObject<Player>()->SetPosition(DirectX::XMFLOAT3(2.0f,0.0f,10.0f));
	//�^���N
	{
		Tank* tank = AddGameObject<Tank>();
		tank->SetPosition(DirectX::XMFLOAT3(34.0f, 0.0f, 4.0f));
	}
	{
		Tank* tank = AddGameObject<Tank>();
		tank->SetPosition(DirectX::XMFLOAT3(34.0f, 0.0f, 8.0f));
	}
	{
		Tank* tank = AddGameObject<Tank>();
		tank->SetPosition(DirectX::XMFLOAT3(34.0f, 0.0f, 12.0f));
	}
	//BGM
	m_BGM = AddGameObject<GameObject>(1)->AddComponent<Audio>();
	m_BGM->Load("asset/audio/BGM.wav");
	m_BGM->Play(true);
	
	m_Transition = AddGameObject<Transition>(4);
	m_Transition->FadeIn();
}
//==============================
//			�㏈��
//==============================
void Game::Uninit()
{
	Explosion::Unload();
}
void Game::Update()
{
	//�v���C���[�����Ȃ���΃t�F�[�h�A�E�g
	if (!m_Player)
	{
		Player* player = GetGameObject<Player>();
		if (player == nullptr)
		{
			m_Player = true;
			Invoke([=]() { m_Transition->FadeOut(); }, 2000);
		}
	}
	//�^���N�����Ȃ���΃t�F�[�h�A�E�g
	if (!m_Enemy)
	{
		Tank* tank = GetGameObject<Tank>();
		if (tank == NULL)
		{
			m_Enemy = true;
			Invoke([=]() { m_Transition->FadeOut2(); }, 1500);
		}
	}
	//�V�[���J��
	if (m_Transition->GetState() == Transition::State::Finish)
	{
		Manager::SetScene<Title>();
		return;
	}
	if (m_Transition->GetState() == Transition::State::Title)
	{
		Manager::SetScene<Result>();
		return;
	}
	
}

