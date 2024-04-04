#pragma once
#include "scene.h"
class Result :public Scene
{
private:
	//Transition
	class Transition* m_Transition{};

	void Init() override;	//‰Šú‰»
	void Update() override;	//XV
};

