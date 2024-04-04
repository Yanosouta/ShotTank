#include "AStar.h"
#include <thread>
#include <future>
//ImGui
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"

//============================================
//				������
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
//				�m�[�h�̍쐬
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

			//�אڃm�[�h�̒ǉ�
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
//			�N���[�Y���X�g�`�F�b�N
//============================================
AStar::EraseResult AStar::EraseNode(std::list<Node*>& list, Node * newNode, float newCost)
{
	for (auto itr = list.begin(); itr != list.end(); itr++)
	{
		//�m�[�h�Ɠ����Z�������邩���ׂ�
		if (IsEqualCell(newNode->Pos, (*itr)->Pos) == true)
		{
			//�R�X�g�̔�r
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
//				�I�[�v�����X�g
//============================================
bool AStar::AddAdjacentNode(std::list<Node*>& openList, std::list<Node*>& closeList, Node * adjacentNode, float cost)
{
	bool canUpdate = true;

	std::list<Node*> nodeList[] = { closeList,
									openList };

	for (std::list<Node*>& list : nodeList)
	{
		//���X�g�ɓ����m�[�h�������ă��X�g�̕����R�X�g�������Ȃ�폜
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
//			�@�@�}���`�X���b�h
//============================================
void AStar::AStarSearch(DirectX::XMINT2 start, DirectX::XMINT2 goal)
{
	//AStar��ʂ̃X���b�h�Ŏ��s
	f = std::async(std::launch::async, 
					&AStar::AStarthread, 
					this, start, goal);
}
//============================================
//				AStar�̎��s
//============================================
std::list<DirectX::XMINT2> AStar::AStarthread(DirectX::XMINT2 start, DirectX::XMINT2 goal)
{
	std::list<Node*> openList;
	std::list<Node*> closeList;

	const Node* goalNode = &m_Map[goal.y][goal.x];

	//�X�V�����m�[�h�ʒu�ۑ��p
	m_lastUpdateCells[MAP_HEIGHT][MAP_WIDTH];

	//�O���t�̏�����
	InitCost(INFINITY, 0);

	//�X�^�[�g�m�[�h�̎w��
	openList.push_back(&m_Map[start.y][start.x]);

	//�o�H�T����
	int count = 0;

	//�I�[�v�����X�g���Ȃ��Ȃ�܂ŉ񂷁B
	while (openList.empty() == false)
	{
		//Node* node = nullptr;
		count++;


		Node* searchNode = (*openList.begin());
		//�T�����X�g���珜�O
		openList.erase(openList.begin());

		//�S�[���ɓ��B������I���
		if (IsEqualCell(searchNode->Pos, goal) == true)
		{
			//�N���[�Y���X�g�ɍŌ�̃m�[�h��ǉ�����
			closeList.push_back(searchNode);
			break;
		}

		for (Node* adjacentNode : searchNode->AdjacentNodes)
		{

			//�v�Z���s���Ă��Ȃ������ꍇ�����v�Z
			if (adjacentNode->HeuristicCost == INFINITY)
			{
				//�q���[���X�e�B�b�N�R�X�g�v�Z
				adjacentNode->HeuristicCost = CalculateHeuristic(adjacentNode, goalNode);
			}

			//�m�[�h�ԃR�X�g
			float edgeCost = CostTable[adjacentNode->Pos.y][adjacentNode->Pos.x];
			//�擾�m�[�h�̃g�[�^���R�X�g
			float nodeCost = searchNode->TotalCost;
			/*
				�g�[�^���R�X�g�Z�o
					�m�[�h�ԃR�X�g�@�{�@�q���[���X�e�B�b�N�R�X�g�@�{�@�擾�m�[�h�̃g�[�^���R�X�g
			*/
			float totalCost = edgeCost + adjacentNode->HeuristicCost * nodeCost;

			//�m�[�h�̒ǉ�
			if (AddAdjacentNode(openList, closeList, adjacentNode, totalCost) == true)
			{
				//�g�[�^���R�X�g���X�V
				adjacentNode->TotalCost = totalCost;

				//�o�H���X�V�����Z����ۑ�
				m_lastUpdateCells[adjacentNode->Pos.y][adjacentNode->Pos.x] = searchNode->Pos;
			}
		}
		bool isAddClose = true;

		//�N���[�Y���X�g�`�F�b�N
		for (auto itr = closeList.begin(); itr != closeList.end(); ++itr)
		{

			//�m�[�h�Ɠ����Z�������邩���ׂ�
			if (IsEqualCell(searchNode->Pos, (*itr)->Pos) == true)
			{
				isAddClose = false;
				break;
			}
		}

		//�����m�[�h���Ȃ�������ǉ�
		if (isAddClose == true)
		{
			//���̃m�[�h�̒T���I��
			closeList.push_back(searchNode);
		}
		//�����\�[�g
		//openList.sort(LessSort(searchNode, node));
	}
	//�o�H����
	std::list<DirectX::XMINT2> routeList;

	//�S�[���Z�����畜������
	routeList.push_back(goal);
	while (routeList.empty() == false)
	{
		DirectX::XMINT2 route = routeList.front();

		//�X�^�[�g�Z���Ȃ�I��
		if (IsEqualCell(route, start) == false)
		{
			if (IsCellWithinTheRange(route.x, route.y) == true)
			{
				//�ǉ�
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

