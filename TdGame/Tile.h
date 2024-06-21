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
	/* 索引瓦片图集的第几位 */
	int terrian = 0;
	/* 是否有装饰物 */
	int decoration = -1;
	/* 方向 */
	Direction direction = Direction::None;
	/*
	* 特殊标志
	* -1：没有
	* 0：房屋
	* 1-9：刷怪点
	*/
	int special_flag = -1;
	bool has_tower = false;

};

typedef std::vector<std::vector<Tile>> TileMap;

