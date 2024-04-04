#pragma once
#include "main.h"
#include "gameObject.h"
class CreateBall : public GameObject
{
public:

	void Init() override;
	void Update() override;
	void ChangeColor();
	void ReturnColor();
private:


};