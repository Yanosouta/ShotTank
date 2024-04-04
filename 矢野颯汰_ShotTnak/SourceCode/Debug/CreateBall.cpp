#include "CreateBall.h"
#include "manager.h"
#include "modelRenderer.h"
#include "shader.h"

//ImGui
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"

void CreateBall::Init()
{
	AddComponent<Shader>()->Load("shader/vertexLightingVS.cso",
		"shader/vertexLightingPS.cso");
	AddComponent<ModelRenderer>()->Load("asset/model/bullet.obj");


}

void CreateBall::Update()
{
	//ImGui::Begin("Line");
	//ImGui::Text("Scale: x:%.2f,y:%.2f,z:%.2f", m_Scale.x, m_Scale.y, m_Scale.z);
	//ImGui::Text("Pos: x:%.2f,y:%.2f,z:%.2f", m_Position.x, m_Position.y, m_Position.z);
	//ImGui::Text("Rot: x:%.2f", m_Rotation.y);
	//ImGui::End();
}

void CreateBall::ChangeColor()
{
	GetComponent<ModelRenderer>()->Load("asset/model/Debugbullet.obj");
}

void CreateBall::ReturnColor()
{
	GetComponent<ModelRenderer>()->Load("asset/model/bullet.obj");
}
