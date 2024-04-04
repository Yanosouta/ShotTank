#include "CreateLine.h"
#include "manager.h"
#include "modelRenderer.h"
#include "shader.h"

//ImGui
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"

void CreateLine::Init()
{
	AddComponent<Shader>()->Load("shader/vertexLightingVS.cso",
		"shader/vertexLightingPS.cso");
	AddComponent<ModelRenderer>()->Load("asset/model/Line.obj");
	
	
}

void CreateLine::Update()
{
	//ImGui::Begin("Line");
	//ImGui::Text("Scale: x:%.2f,y:%.2f,z:%.2f", m_Scale.x, m_Scale.y, m_Scale.z);
	//ImGui::Text("Pos: x:%.2f,y:%.2f,z:%.2f", m_Position.x, m_Position.y, m_Position.z);
	//ImGui::Text("Rot: x:%.2f", m_Rotation.y);
	//ImGui::End();
}

void CreateLine::PosCreate(DirectX::XMFLOAT3 pos1, DirectX::XMFLOAT3 pos2)
{
	DirectX::XMFLOAT3 scale;
	DirectX::XMFLOAT3 angle;
	float RotY;
	//中心座標
	m_Position.x = (pos1.x + pos2.x) / 2;
	m_Position.y = (pos1.y + pos2.y) / 2;
	m_Position.z = (pos1.z + pos2.z) / 2;
	//サイズ
	scale.x = abs(pos2.x - pos1.x);
	scale.y = abs(pos2.y - pos1.y);
	scale.z = abs(pos2.z - pos1.z);
	
	if (scale.x > scale.z){
		m_Scale.x = abs(pos2.x - pos1.x) / 2.0f;
		m_Scale.y = 0.1f;
		m_Scale.z = 0.1f;
	}
	else {
		m_Scale.x = abs(pos2.z - pos1.z) / 2.0f;
		m_Scale.y = 0.1f;
		m_Scale.z =  0.1f;
	}
	//2つの座標から角度を計算
	angle.x = pos2.x - pos1.x;
	angle.z = pos2.z - pos1.z;
	RotY = atan2(angle.z, angle.x);
	m_Rotation.y = -RotY ;
	
}
