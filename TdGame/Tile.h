#pragma once
#define SIZE_TILE 48

#include <vector>

struct Tile
{
	enum class Direction
	{
		None=0,
		Up,
		Down,
		Left,
		Right
	};
	/* ������Ƭͼ���ĵڼ�λ */
	int terrian = 0;
	/* �Ƿ���װ���� */
	int decoration = -1;
	/* ���� */
	Direction direction = Direction::None;
	/*
	* �����־
	* -1��û��
	* 0������
	* 1-9��ˢ�ֵ�
	*/
	int special_flag = -1;
	bool has_tower = false;

};

typedef std::vector<std::vector<Tile>> TileMap;

