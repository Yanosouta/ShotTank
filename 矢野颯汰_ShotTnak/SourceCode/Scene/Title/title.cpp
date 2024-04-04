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
//			初期化
//============================
void Title::Init()
{
	GameObject* title = AddGameObject<GameObject>(3);					//タイトル							
	title->AddComponent<Shader>()->Load("shader/unlitTextureVS.cso", 	//シェーダ
		"shader/unlitTexturePS.cso");
	AddGameObject<TitleCamera>();	//カメラ
	AddGameObject<Sky>();			//背景
	AddGameObject<TitleObj>();		//オブジェクト
	AddGameObject<TitleLogo>();		//テキスト
	title->AddComponent<Sprite>()->Init(650, 0,		//2D表示
		SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 
		"asset/texture/TitleLogo.png");
	
	//フェード
	m_Transition = AddGameObject<Transition>(4);
	m_Transition->FadeIn();
}
//============================
//			更新
//============================
void Title::Update()
{
	//シーン遷移
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