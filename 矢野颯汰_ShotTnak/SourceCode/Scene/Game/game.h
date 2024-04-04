#pragma once

#include "scene.h"

class Game : public Scene
{
private:
	//BGM
	class Audio* m_BGM{};
	//Transition
	class Transition* m_Transition{};
	//�ϐ���`
	bool m_Player = false;	//�v���C���[
	bool m_Enemy = false;	//�^���N
public:
	void Init() override;	//������
	void Uninit() override;	//�㏈��
	void Update() override;	//�X�V
};
