#pragma once
#include "scene.h"
class Result :public Scene
{
private:
	//Transition
	class Transition* m_Transition{};

	void Init() override;	//初期化
	void Update() override;	//更新
};

