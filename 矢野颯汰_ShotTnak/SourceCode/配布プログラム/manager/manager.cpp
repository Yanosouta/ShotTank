// [manager.cpp]
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "title.h"
#include "audio.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"

Scene* Manager::m_Scene{};

void Manager::Init()
{
	Renderer::Init();
	Input::Init();
	Audio::InitMaster();
	SetScene<Title>();
}

void Manager::Uninit()
{
	m_Scene->UninitBase();
	delete m_Scene;

	Audio::UninitMaster();
	Input::Uninit();
	Renderer::Uninit();
}

void Manager::Update()
{
	Input::Update();
	//ImGui
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	//ImGui::Begin("test");
	//ImGui::End();

	m_Scene->UpdateBase();
}

void Manager::Draw()
{
	Renderer::Begin();

	m_Scene->DrawBase();

	Renderer::End();
}
