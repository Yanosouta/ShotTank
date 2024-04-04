#pragma once

#include "renderer.h"

#include <list>
#include "component.h"

class GameObject
{

protected:

	DirectX::XMFLOAT3 m_Position = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	DirectX::XMFLOAT3 m_Rotation = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	DirectX::XMFLOAT3 m_Velocity = DirectX::XMFLOAT3(1.0f, 0.0f, 1.0f);
	DirectX::XMFLOAT3 m_Scale = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
	DirectX::XMFLOAT3 m_Diffuse = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
	DirectX::XMFLOAT3 m_Speed = DirectX::XMFLOAT3(0.1f, 0.1f, 0.1f);
	DirectX::XMFLOAT3 m_Forward = DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f);
	
	std::list<Component*> m_Component;
	std::list<GameObject*> m_ChildGameObject;

	bool m_Destroy = false;
	bool m_CollisionFlg = false;

public:
	GameObject() {}
	virtual ~GameObject() {}


	DirectX::XMFLOAT3 GetPosition() { return m_Position; }
	DirectX::XMFLOAT3 GetRotation() { return m_Rotation; }
	DirectX::XMFLOAT3 GetScale() { return m_Scale; }
	DirectX::XMFLOAT3 GetVelocity() { return m_Velocity; }
	DirectX::XMFLOAT3 GetSpeed() { return m_Speed; }
	bool GetCollisionFlg() { return m_CollisionFlg; }

	void SetDiffuse(DirectX::XMFLOAT3 diffuse) { m_Diffuse = diffuse; }
	void SetPosition(DirectX::XMFLOAT3 Position) { m_Position = Position; }
	void SetRotation(DirectX::XMFLOAT3 Rotation) { m_Rotation = Rotation; }
	void SetScale(DirectX::XMFLOAT3 Scale) { m_Scale = Scale; }
	void SetVelocity(DirectX::XMFLOAT3 Velocity) {m_Velocity = Velocity; }
	void SetSpeed(DirectX::XMFLOAT3 speed) { m_Speed = speed; }
	void SetDestroy() { m_Destroy = true; }
	void SetCollisionFlg(bool flg) {m_CollisionFlg = flg; }
	DirectX::XMFLOAT3 GetForward()//前方向ベクトル取得
	{
		DirectX::XMFLOAT4X4 rot;
		DirectX::XMStoreFloat4x4(&rot,
			DirectX::XMMatrixRotationRollPitchYaw(
				m_Rotation.x, m_Rotation.y, m_Rotation.z));

		DirectX::XMFLOAT3 forward;
		forward.x = rot._31;
		forward.y = rot._32;
		forward.z = rot._33;

		return forward;
	}

	bool Destroy()
	{
		if (m_Destroy) {
			UninitBase();
			delete this;
			return true;
		}
		return false;
	}


	virtual void Init() {}
	virtual void Uninit() {}
	virtual void Update() {}
	virtual void Draw() {}






	template <typename T>
	T* AddComponent()
	{
		T* component = new T(this);
		m_Component.push_back(component);
		((Component*)component)->Init();

		return component;
	}


	template <typename T>
	T* GetComponent()
	{
		for (Component* component : m_Component)
		{
			if (typeid(*component) == typeid(T))
			{
				return (T*)component;
			}
		}
		return nullptr;
	}

	template <typename T>
	T* AddChild()
	{
		T* child = new T();
		m_ChildGameObject.push_back(child);
		child->InitBase();

		return child;
	}

	void InitBase()
	{
		Init();
	}


	void UninitBase()
	{
		Uninit();

		for (Component* component : m_Component)
		{
			component->Uninit();
			delete component;
		}
		m_Component.clear();
	}

	void UpdateBase()
	{

		for (Component* component : m_Component)
		{
			component->Update();
		}

		Update();
	}

	void DrawBase(DirectX::XMFLOAT4X4 ParentMatrix)
	{
		// マトリクス設定
		DirectX::XMFLOAT4X4 world;
		DirectX::XMMATRIX scale, rot, trans;
		scale = DirectX::XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
		rot = DirectX::XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
		trans = DirectX::XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
		DirectX::XMStoreFloat4x4(&world, scale * rot * trans * DirectX::XMLoadFloat4x4(&ParentMatrix));

		for (GameObject* child : m_ChildGameObject)
		{
			child->DrawBase(world);
		}

		Renderer::SetWorldMatrix(&world);

		for (Component* component : m_Component)
		{
			component->Draw();
		}

		Draw();
	}


};
