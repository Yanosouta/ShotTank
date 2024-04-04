#include "wall.h"
#include "modelRenderer.h"
#include "shader.h"
void Wall::Init()
{
	AddComponent<Shader>()->Load("shader/vertexLightingVS.cso", "shader/vertexLightingPS.cso");
	AddComponent<ModelRenderer>()->Load("asset/model/wall.obj");
}
