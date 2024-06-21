#include "Route.h"

/**
 * @brief      ����һ������ʼλ�õ�home�Ŀ���·��
 *
 * @param[in]	map ��Ϸ��ͼ
 * @param[in]	origin_pos ��ʼλ��
 * @param[out]
 * @return
 */

Route::Route(const TileMap& map, const SDL_Point& origin_pos)
{
	size_t map_width = map[0].size();
	size_t map_height = map.size();
	SDL_Point next_pos = origin_pos;

	while (true)
	{
		/* ������ͼ�߽����˳� */
		if (next_pos.x >= map_width || next_pos.y >= map_height)
			break;

		/* ѭ��·�����˳� */
		if (is_loop_route(idx_list, next_pos))
			break;
		else
			idx_list.push_back(next_pos);

		bool is_next_dir_exist = true;

		const Tile& tile = map[next_pos.y][next_pos.x];

		/* �����������˳� */
		if (tile.special_flag == 0)
			break;

		switch (tile.direction)
		{
		case Tile::Direction::Up:
			next_pos.y--;
			break;
		case Tile::Direction::Down:
			next_pos.y++;
			break;
		case Tile::Direction::Left:
			next_pos.x--;
			break;
		case Tile::Direction::Right:
			next_pos.x++;
			break;
		default:
			is_next_dir_exist = false;
			break;
		}

		if (!is_next_dir_exist)
			break;
			
	}
}

const Route::IdxList& Route::get_idx_list() const
{
	return idx_list;
}

bool Route::is_loop_route(const IdxList& idx_list, const SDL_Point& cur_point)
{
	if (idx_list.empty())
		return false;
	for (const SDL_Point& item:idx_list)
	{
		if (item.x == cur_point.x && item.y == cur_point.y)
			return true;
	}
	return false;
}
