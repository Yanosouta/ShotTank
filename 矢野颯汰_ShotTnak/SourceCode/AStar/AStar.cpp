#include "AStar.h"
#include <thread>
#include <future>
//ImGui
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"

//============================================
//				初期化
//============================================
void AStar::Init()
{
	for (int y = 0; y < MAP_HEIGHT; ++y)
	{
		for (int x = 0; x < MAP_WIDTH; ++x)
		{
			m_lastUpdateCells[y][x] = {0,0};
		}
	}

}
//============================================
//				ノードの作成
//============================================
void AStar::CreateMap()
{
	for (int y = 0; y < MAP_HEIGHT; ++y)
	{
		for (int x = 0; x < MAP_WIDTH; ++x)
		{
			m_Map[y][x].Pos.x = x;
			m_Map[y][x].Pos.y = y;
			DirectX::XMINT2 adjacentCells[4];
			adjacentCells[0] = { x, y - 1 };
			adjacentCells[1] = { x - 1,y };
			adjacentCells[2] = { x + 1,y };
			adjacentCells[3] = { x, y + 1 };

			//隣接ノードの追加
			for (DirectX::XMINT2 cell : adjacentCells)
			{
				if (IsCellWithinTheRange(cell.x, cell.y) == true &&
					CostTable[cell.y][cell.x] == 1)
				{
					m_Map[y][x].AdjacentNodes.push_back(&m_Map[cell.y][cell.x]);
				}
			}
		}
	}
}
//============================================
//			クローズリストチェック
//============================================
AStar::EraseResult AStar::EraseNode(std::list<Node*>& list, Node * newNode, float newCost)
{
	for (auto itr = list.begin(); itr != list.end(); itr++)
	{
		//ノードと同じセルがあるか調べる
		if (IsEqualCell(newNode->Pos, (*itr)->Pos) == true)
		{
			//コストの比較
			if (newCost < (*itr)->TotalCost)
			{
				list.erase(itr);
				return EraseResult::Deletion;
			}
			else
			{
				return EraseResult::CannotDel;
			}
		}
	}
	return EraseResult::NotFound;
}
//============================================
//				オープンリスト
//============================================
bool AStar::AddAdjacentNode(std::list<Node*>& openList, std::list<Node*>& closeList, Node * adjacentNode, float cost)
{
	bool canUpdate = true;

	std::list<Node*> nodeList[] = { closeList,
									openList };

	for (std::list<Node*>& list : nodeList)
	{
		//リストに同じノードがあってリストの方がコストが高いなら削除
		if (EraseNode(list, adjacentNode, cost) == EraseResult::CannotDel)
		{
			canUpdate = false;
		}
	}
	if (canUpdate == true)
	{
		openList.push_back(adjacentNode);
		return true;
	}
	return false;
}


//============================================
//			　　マルチスレッド
//============================================
void AStar::AStarSearch(DirectX::XMINT2 start, DirectX::XMINT2 goal)
{
	//AStarを別のスレッドで実行
	f = std::async(std::launch::async, 
					&AStar::AStarthread, 
					this, start, goal);
}
//============================================
//				AStarの実行
//============================================
std::list<DirectX::XMINT2> AStar::AStarthread(DirectX::XMINT2 start, DirectX::XMINT2 goal)
{
	std::list<Node*> openList;
	std::list<Node*> closeList;

	const Node* goalNode = &m_Map[goal.y][goal.x];

	//更新したノード位置保存用
	m_lastUpdateCells[MAP_HEIGHT][MAP_WIDTH];

	//グラフの初期化
	InitCost(INFINITY, 0);

	//スタートノードの指定
	openList.push_back(&m_Map[start.y][start.x]);

	//経路探索回数
	int count = 0;

	//オープンリストがなくなるまで回す。
	while (openList.empty() == false)
	{
		//Node* node = nullptr;
		count++;


		Node* searchNode = (*openList.begin());
		//探索リストから除外
		openList.erase(openList.begin());

		//ゴールに到達したら終わり
		if (IsEqualCell(searchNode->Pos, goal) == true)
		{
			//クローズリストに最後のノードを追加する
			closeList.push_back(searchNode);
			break;
		}

		for (Node* adjacentNode : searchNode->AdjacentNodes)
		{

			//計算を行っていなかった場合だけ計算
			if (adjacentNode->HeuristicCost == INFINITY)
			{
				//ヒューリスティックコスト計算
				adjacentNode->HeuristicCost = CalculateHeuristic(adjacentNode, goalNode);
			}

			//ノード間コスト
			float edgeCost = CostTable[adjacentNode->Pos.y][adjacentNode->Pos.x];
			//取得ノードのトータルコスト
			float nodeCost = searchNode->TotalCost;
			/*
				トータルコスト算出
					ノード間コスト　＋　ヒューリスティックコスト　＋　取得ノードのトータルコスト
			*/
			float totalCost = edgeCost + adjacentNode->HeuristicCost * nodeCost;

			//ノードの追加
			if (AddAdjacentNode(openList, closeList, adjacentNode, totalCost) == true)
			{
				//トータルコストを更新
				adjacentNode->TotalCost = totalCost;

				//経路を更新したセルを保存
				m_lastUpdateCells[adjacentNode->Pos.y][adjacentNode->Pos.x] = searchNode->Pos;
			}
		}
		bool isAddClose = true;

		//クローズリストチェック
		for (auto itr = closeList.begin(); itr != closeList.end(); ++itr)
		{

			//ノードと同じセルがあるか調べる
			if (IsEqualCell(searchNode->Pos, (*itr)->Pos) == true)
			{
				isAddClose = false;
				break;
			}
		}

		//同じノードがなかったら追加
		if (isAddClose == true)
		{
			//このノードの探索終了
			closeList.push_back(searchNode);
		}
		//昇順ソート
		//openList.sort(LessSort(searchNode, node));
	}
	//経路復元
	std::list<DirectX::XMINT2> routeList;

	//ゴールセルから復元する
	routeList.push_back(goal);
	while (routeList.empty() == false)
	{
		DirectX::XMINT2 route = routeList.front();

		//スタートセルなら終了
		if (IsEqualCell(route, start) == false)
		{
			if (IsCellWithinTheRange(route.x, route.y) == true)
			{
				//追加
				routeList.push_front(m_lastUpdateCells[route.y][route.x]);
			}
		}
		else
		{
			break;
		}
	}
	return routeList;
}

