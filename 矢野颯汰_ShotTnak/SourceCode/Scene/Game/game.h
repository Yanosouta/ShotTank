#pragma once

#include "scene.h"

class Game : public Scene
{
private:
	//BGM
	class Audio* m_BGM{};
	//Transition
	class Transition* m_Transition{};
	//変数定義
	bool m_Player = false;	//プレイヤー
	bool m_Enemy = false;	//タンク
public:
	void Init() override;	//初期化
	void Uninit() override;	//後処理
	void Update() override;	//更新
};
