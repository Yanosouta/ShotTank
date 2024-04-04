// Boxオブジェクト [box.h]
#pragma once
#include "gameObject.h"
#include "bullet.h"
#include "scene.h"
class Box : public GameObject
{
public:
	void Init() override;
	void Update() override;
private:
	// 頂点情報
	struct Vertex
	{
		DirectX::XMFLOAT3	pos;		// 座標
		DirectX::XMFLOAT3	normal;		// 法線
		DirectX::XMFLOAT2	uv;			// テクスチャ座標
		float				weight[4];	// 頂点ブレンドウェイト
		unsigned int		index[4];	// 頂点ブレンドインデックス
	};
	DirectX::XMFLOAT3 g_Normal[6] = {
		{-1.0f,  0.0f,  0.0f},	// 左
		{ 1.0f,  0.0f,  0.0f},	// 右
		{ 0.0f, -1.0f,  0.0f},	// 下
		{ 0.0f,  1.0f,  0.0f},	// 上
		{ 0.0f,  0.0f, -1.0f},	// 前
		{ 0.0f,  0.0f,  1.0f},	// 後
	};
};


