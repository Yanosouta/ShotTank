
#include "renderer.h"
#include "field.h"
#include "CreateTexture.h"
#include "shader.h"
#include "modelRenderer.h"

void Field::Init()
{
	AddComponent<Shader>()->Load("shader/vertexLightingVS.cso", "shader/vertexLightingPS.cso");
	AddComponent<ModelRenderer>()->Load("asset/model/ground.obj");
}

