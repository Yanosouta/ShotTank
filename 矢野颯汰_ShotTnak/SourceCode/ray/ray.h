#pragma once
#include "component.h"
#include "gameObject.h"
#include "box.h"
#include "scene.h"
class Ray : public Component
{
public:
	//---当たり判定を行う形状
	struct Plane
	{
		DirectX::XMFLOAT3 normal;	//面の法線
		DirectX::XMFLOAT3 pos;		//面上の座標
	};
	struct RRay
	{
		DirectX::XMFLOAT3 start;		//レイの開始点
		DirectX::XMFLOAT3 direction;	//レイの方向
		float			  length;		//レイの長さ
	};
	//---当たり判定の結果
	struct Result
	{
		bool hit;
		DirectX::XMFLOAT3 point;
	};

	struct Triangle
	{
		DirectX::XMFLOAT3 p[3];
	};
	using Component::Component;
public:
	void Init() override;
	void Update() override;
	//---形状ごとの当たり判定を実装
	static Result CheckRayPlane(DirectX::XMFLOAT3, DirectX::XMFLOAT3,DirectX::XMFLOAT3, DirectX::XMFLOAT3,float);
	static Result CheckPointTraiangle(DirectX::XMFLOAT3 point, Triangle triangle);
private:

	Plane plane;
	std::vector <Box*> m_Box;
	Scene* m_Scene;
};

