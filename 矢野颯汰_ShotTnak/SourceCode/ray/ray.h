#pragma once
#include "component.h"
#include "gameObject.h"
#include "box.h"
#include "scene.h"
class Ray : public Component
{
public:
	//---�����蔻����s���`��
	struct Plane
	{
		DirectX::XMFLOAT3 normal;	//�ʂ̖@��
		DirectX::XMFLOAT3 pos;		//�ʏ�̍��W
	};
	struct RRay
	{
		DirectX::XMFLOAT3 start;		//���C�̊J�n�_
		DirectX::XMFLOAT3 direction;	//���C�̕���
		float			  length;		//���C�̒���
	};
	//---�����蔻��̌���
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
	//---�`�󂲂Ƃ̓����蔻�������
	static Result CheckRayPlane(DirectX::XMFLOAT3, DirectX::XMFLOAT3,DirectX::XMFLOAT3, DirectX::XMFLOAT3,float);
	static Result CheckPointTraiangle(DirectX::XMFLOAT3 point, Triangle triangle);
private:

	Plane plane;
	std::vector <Box*> m_Box;
	Scene* m_Scene;
};

