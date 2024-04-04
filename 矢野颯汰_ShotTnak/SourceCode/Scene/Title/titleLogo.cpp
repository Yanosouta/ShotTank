#include "titleLogo.h"
#include "manager.h"
#include "shader.h"
#include "sprite.h"
#include "CreateTexture.h"
//====================
//		������
//====================
void TitleLogo::Init()
{
	AddComponent<Shader>()->Load("shader/unlitTextureVS.cso",	//�V�F�[�_
		"shader/unlitTexturePS.cso");
	AddComponent<Sprite>()->Init(700,300,500,300, "asset/texture/Enter.png");	//2D�\��

}
