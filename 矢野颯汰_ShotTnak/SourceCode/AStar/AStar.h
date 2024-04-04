#pragma once
#include "component.h"
#include <vector>
#include <list>
#include <future>
#include <DirectXMath.h>

#define MAP_HEIGHT 9	//縦
#define MAP_WIDTH 17	//横


class AStar :	public Component
{
private:
	std::list<DirectX::XMINT2> m_result;
	std::future<std::list<DirectX::XMINT2>> f;
	//削除結果
	enum EraseResult
	{
		NotFound,	//未発見
		Deletion,	//削除
		CannotDel,	//削除できない

	};

	struct Node
	{
		DirectX::XMINT2 Pos;				//ノードの座標
		std::vector<Node*> AdjacentNodes;	//隣接ノード
		float HeuristicCost;				//ヒューリスティックコスト
		float TotalCost;					//コスト
	};
	// コストテーブル
	int CostTable[MAP_HEIGHT][MAP_WIDTH] =
	{
		//0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 
		{ 1, 1, 1, 1, 1 ,1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, // 0
		{ 1, 1, 1, 1, 1 ,1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, // 1
		{ 1, 1, 0, 1, 1 ,1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1}, // 2
		{ 1, 1, 0, 1, 1 ,1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1}, // 3
		{ 1, 1, 0, 1, 1 ,1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1}, // 4
		{ 1, 1, 0, 1, 1 ,1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1}, // 5
		{ 1, 1, 0, 1, 1 ,1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1}, // 6
		{ 1, 1, 1, 1, 1 ,1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, // 7
		{ 1, 1, 1, 1, 1 ,1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}  // 8
	};

	//マップ
	Node m_Map[MAP_HEIGHT][MAP_WIDTH];

	DirectX::XMINT2 m_lastUpdateCells[MAP_HEIGHT][MAP_WIDTH];

public:
	using Component::Component;

	AStar() = default;
	//初期化
	void Init();
	//マップの生成
	void CreateMap();
	//AStarを別スレッドで実行するための関数
	void AStarSearch(DirectX::XMINT2 start, DirectX::XMINT2 goal);
	//AStarの実行
	std::list<DirectX::XMINT2> AStarthread(DirectX::XMINT2 start, DirectX::XMINT2 goal);
	//実行結果を渡す
	std::list<DirectX::XMINT2> GetAStar(std::list<DirectX::XMINT2> list) {
		if (f.valid()){	//　fが空だったら引数をそのまま返す(エラー回避のため)
			return  f.get();
		}
		else {
			return list;
		}

		
	}
	//Cellの初期化
	DirectX::XMINT2 InitCell(int x, int y)
	{
		DirectX::XMINT2 cell;
		cell.x = x;
		cell.y = y;
		return cell;
	}
	//ノードの作成関数
	//クローズリストチェック
	EraseResult EraseNode(std::list<Node*>& list, Node* newNode, float newCost);
	//オープンリストに追加
	bool AddAdjacentNode(std::list<Node*>& openList, std::list<Node*>& closeList, Node* adjacentNode, float cost);
	//昇順ソート関数
	bool LessSort(Node* a, Node* b)
	{
		if (a->TotalCost < b->TotalCost)
			return true;
		return false;
	}
	//マップ範囲内かチェック関数
	bool IsCellWithinTheRange(int x, int y)
	{
		if (x >= 0 && x < MAP_WIDTH && y >= 0 && y < MAP_HEIGHT)
			return true;
		return false;
	}
	//コストの初期化		
	//hcost:ヒューリスティックコスト
	//tcost:トータルコスト
	void InitCost(int hcost, int tcost)
	{
		for (int y = 0; y < MAP_HEIGHT; y++)
		{
			for (int x = 0; x < MAP_WIDTH; x++)
			{
				m_Map[y][x].HeuristicCost = hcost;
				m_Map[y][x].TotalCost = tcost;
			}
		}
	}
	//ヒューリスティックコスト計算(ノードとゴールまでの距離を返している
	float CalculateHeuristic(const Node* node, const Node* goal)
	{
		float x = fabsf(goal->Pos.x - node->Pos.x);
		float y = fabsf(goal->Pos.y - node->Pos.y);

		return sqrtf(x * x + y * y);
	}
	//セルの比較
	bool IsEqualCell(DirectX::XMINT2  a, DirectX::XMINT2 b)
	{
		if (a.x == b.x &&a.y == b.y)
			return true;
		return false;
	}

	

};

