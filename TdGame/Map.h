#pragma once
#include "Tile.h"
#include "Route.h"

#include <SDL.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <unordered_map>


class Map
{
public:
	typedef std::unordered_map<int, Route> SpawnerRoutePool;
public:
	Map() = default;
	~Map() = default;

	bool load(const std::string& path);
	void load_tile_from_string(Tile& tile,const std::string& str);
	size_t get_width() const;
	size_t get_height() const;

	const TileMap& get_tile_map() const;
	const SDL_Point& get_home_point() const;
	const SpawnerRoutePool& get_spawner_route_pool() const;
	void place_tower(const SDL_Point& idx_tile);


private:
	TileMap tile_map;

	/* »º´æµØÍ¼Êý¾Ý */
	SDL_Point idx_home = { 0 };
	SpawnerRoutePool spawner_route_pool;
private:
	std::string trim_str(const std::string& str);
	void generate_map_cache();
};

