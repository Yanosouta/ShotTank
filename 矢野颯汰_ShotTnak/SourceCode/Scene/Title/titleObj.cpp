#include "titleObj.h"
#include "modelRenderer.h"
#include "shader.h"
void TitleObj::Init()
{
	AddComponent<Shader>()->Load("shader/vertexLightingVS.cso",
		"shader/vertexLightingPS.cso");
	AddComponent<ModelRenderer>()->Load("asset/model/title.obj");
}
