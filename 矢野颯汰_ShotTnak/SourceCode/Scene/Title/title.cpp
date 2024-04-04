#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "title.h"
#include "game.h"
#include "shader.h"
#include "sprite.h"
#include "titleCamera.h"
#include "sky.h"
#include "titleObj.h"
#include "titleLogo.h"
#include "modelRenderer.h"
#include "transition.h"


//============================
//			������
//============================
void Title::Init()
{
	GameObject* title = AddGameObject<GameObject>(3);					//�^�C�g��							
	title->AddComponent<Shader>()->Load("shader/unlitTextureVS.cso", 	//�V�F�[�_
		"shader/unlitTexturePS.cso");
	AddGameObject<TitleCamera>();	//�J����
	AddGameObject<Sky>();			//�w�i
	AddGameObject<TitleObj>();		//�I�u�W�F�N�g
	AddGameObject<TitleLogo>();		//�e�L�X�g
	title->AddComponent<Sprite>()->Init(650, 0,		//2D�\��
		SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 
		"asset/texture/TitleLogo.png");
	
	//�t�F�[�h
	m_Transition = AddGameObject<Transition>(4);
	m_Transition->FadeIn();
}
//============================
//			�X�V
//============================
void Title::Update()
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
		Manager::SetScene<Game>();
	}
}