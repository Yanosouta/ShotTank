#include "result.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "title.h"
#include "shader.h"
#include "sprite.h"
#include "transition.h"
//======================
//		������
//======================
void Result::Init()
{
	GameObject* titleLogo = AddGameObject<GameObject>(3);				//�^�C�g��
	titleLogo->AddComponent<Shader>()->Load("shader/unlitTextureVS.cso",//�V�F�[�_
		"shader/unlitTexturePS.cso");
	titleLogo->AddComponent<Sprite>()->Init(0, 0,						//2D�\��
		SCREEN_WIDTH, SCREEN_HEIGHT, 
		"asset/texture/GameClear.png");
	
	//Transition
	m_Transition = AddGameObject<Transition>(4);
	m_Transition->FadeIn();

}
//========================
//			�X�V
//========================
void Result::Update()
{
	//�V�[���J��
	if (m_Transition->GetState() == Transition::State::Stop)
	{
		if (Input::GetKeyTrigger(VK_RETURN) || Input::GetKeyTrigger(VK_SPACE))
		{
			m_Transition->FadeOut();
		}
	}
	if (m_Transition->GetState() == Transition::State::Finish)
	{
		Manager::SetScene<Title>();
	}

}