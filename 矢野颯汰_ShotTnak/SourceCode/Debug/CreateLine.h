#pragma once
#include "main.h"
#include "gameObject.h"
class CreateLine : public GameObject
{
public:
	
	void Init() override;
	void Update() override;
	//���W�Q�ŕ`��
	void PosCreate(DirectX::XMFLOAT3, DirectX::XMFLOAT3);
private:

	
};
