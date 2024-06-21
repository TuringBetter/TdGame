#pragma once
#include "Tile.h"
#include <vector>
#include <SDL.h>
class Route
{
public:
	typedef std::vector<SDL_Point> IdxList;
public:
	Route() = default;
	~Route() = default;
	Route(const TileMap& map, const SDL_Point& origin_pos);

	const IdxList& get_idx_list() const;
private:
	IdxList idx_list;
private:
	bool is_loop_route(const IdxList& idx_list,const SDL_Point& cur_point);
};

