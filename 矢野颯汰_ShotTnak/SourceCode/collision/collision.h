#pragma once
#include "component.h"
#include "gameObject.h"
class Collision : public Component
{
private:
public:
	
	using Component::Component;


	struct Vertices
	{
		DirectX::XMFLOAT3 pos[8];
	};
	struct Triangle
	{
		DirectX::XMFLOAT3 p[3];
	};
	Vertices GetVertices();


};

