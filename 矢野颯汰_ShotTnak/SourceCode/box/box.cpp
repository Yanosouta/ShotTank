#include "manager.h"
#include "modelRenderer.h"
#include "box.h"
#include "shader.h"
#include "collision.h"


void Box::Init()
{
	AddComponent<Shader>()->Load("shader/vertexLightingVS.cso",
		"shader/vertexLightingPS.cso");
	AddComponent<ModelRenderer>()->Load("asset/model/boxtest.obj");
	
	
}

void Box::Update()
{

	
	
}



