#pragma once
#include "component.h"
#include <vector>
#include <list>
#include <future>
#include <DirectXMath.h>

#define MAP_HEIGHT 9	//�c
#define MAP_WIDTH 17	//��


class AStar :	public Component
{
private:
	std::list<DirectX::XMINT2> m_result;
	std::future<std::list<DirectX::XMINT2>> f;
	//�폜����
	enum EraseResult
	{
		NotFound,	//������
		Deletion,	//�폜
		CannotDel,	//�폜�ł��Ȃ�

	};

	struct Node
	{
		DirectX::XMINT2 Pos;				//�m�[�h�̍��W
		std::vector<Node*> AdjacentNodes;	//�אڃm�[�h
		float HeuristicCost;				//�q���[���X�e�B�b�N�R�X�g
		float TotalCost;					//�R�X�g
	};
	// �R�X�g�e�[�u��
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

	//�}�b�v
	Node m_Map[MAP_HEIGHT][MAP_WIDTH];

	DirectX::XMINT2 m_lastUpdateCells[MAP_HEIGHT][MAP_WIDTH];

public:
	using Component::Component;

	AStar() = default;
	//������
	void Init();
	//�}�b�v�̐���
	void CreateMap();
	//AStar��ʃX���b�h�Ŏ��s���邽�߂̊֐�
	void AStarSearch(DirectX::XMINT2 start, DirectX::XMINT2 goal);
	//AStar�̎��s
	std::list<DirectX::XMINT2> AStarthread(DirectX::XMINT2 start, DirectX::XMINT2 goal);
	//���s���ʂ�n��
	std::list<DirectX::XMINT2> GetAStar(std::list<DirectX::XMINT2> list) {
		if (f.valid()){	//�@f���󂾂�������������̂܂ܕԂ�(�G���[����̂���)
			return  f.get();
		}
		else {
			return list;
		}

		
	}
	//Cell�̏�����
	DirectX::XMINT2 InitCell(int x, int y)
	{
		DirectX::XMINT2 cell;
		cell.x = x;
		cell.y = y;
		return cell;
	}
	//�m�[�h�̍쐬�֐�
	//�N���[�Y���X�g�`�F�b�N
	EraseResult EraseNode(std::list<Node*>& list, Node* newNode, float newCost);
	//�I�[�v�����X�g�ɒǉ�
	bool AddAdjacentNode(std::list<Node*>& openList, std::list<Node*>& closeList, Node* adjacentNode, float cost);
	//�����\�[�g�֐�
	bool LessSort(Node* a, Node* b)
	{
		if (a->TotalCost < b->TotalCost)
			return true;
		return false;
	}
	//�}�b�v�͈͓����`�F�b�N�֐�
	bool IsCellWithinTheRange(int x, int y)
	{
		if (x >= 0 && x < MAP_WIDTH && y >= 0 && y < MAP_HEIGHT)
			return true;
		return false;
	}
	//�R�X�g�̏�����		
	//hcost:�q���[���X�e�B�b�N�R�X�g
	//tcost:�g�[�^���R�X�g
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
	//�q���[���X�e�B�b�N�R�X�g�v�Z(�m�[�h�ƃS�[���܂ł̋�����Ԃ��Ă���
	float CalculateHeuristic(const Node* node, const Node* goal)
	{
		float x = fabsf(goal->Pos.x - node->Pos.x);
		float y = fabsf(goal->Pos.y - node->Pos.y);

		return sqrtf(x * x + y * y);
	}
	//�Z���̔�r
	bool IsEqualCell(DirectX::XMINT2  a, DirectX::XMINT2 b)
	{
		if (a.x == b.x &&a.y == b.y)
			return true;
		return false;
	}

	

};

