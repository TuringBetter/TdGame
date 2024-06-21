#include "Map.h"

/*
* @brief  从csv中加载瓦片地图
*    x---------------------->
* y  0\1\2\3,  0\1\2\3,   0\1\2\3,   0\1\2\3
* |  0\1\2\3,  0\1\2\3,   0\1\2\3,   0\1\2\3
* |  0\1\2\3,  0\1\2\3,   0\1\2\3,   0\1\2\3
* |  0\1\2\3,  0\1\2\3,   0\1\2\3,   0\1\2\3
* |  0\1\2\3,  0\1\2\3,   0\1\2\3,   0\1\2\3
* |  0\1\2\3,  0\1\2\3,   0\1\2\3,   0\1\2\3
* |  0\1\2\3,  0\1\2\3,   0\1\2\3,   0\1\2\3
* |
* ↓
* @param[in] path
* @param[out]
* @return
*/
bool Map::load(const std::string& path)
{
    std::ifstream file(path);
    if (!file.good()) 
        return false;

    TileMap tile_map_tmp;

    int idx_x = -1, idx_y = -1;

    std::string str_line;
    while(std::getline(file,str_line))
    {
        /* 去除该行的首尾空格 */
        str_line = trim_str(str_line);
        /* 如果读取到了空行,则跳过 */
        if (str_line.empty())
            continue;
        idx_x = -1, idx_y++;
        /* 在行尾添加新的一行 */
        tile_map_tmp.emplace_back();

        /* 
        *  处理读取到的行的瓦片数据 
        *  即0\1\2\3,  0\1\2\3,   0\1\2\3,   0\1\2\3
        */
        std::string str_tile;
        std::stringstream str_stream(str_line);
        while(std::getline(str_stream,str_tile,','))
        {
            idx_x++;
            tile_map_tmp[idx_y].emplace_back();
            Tile& tile = tile_map_tmp[idx_y].back();
            load_tile_from_string(tile, str_tile);
        }

    }

    file.close();

    /* 如果地图为空，则返回错误 */
    if (tile_map_tmp.size() || tile_map_tmp[0].empty())
        return false;

    tile_map = tile_map_tmp;

    generate_map_cache();

    return false;
}

/**
 * @brief 从字符串中解析数据 
 *        10\-1\0\-100  ---->  tile{decoration,direction,special_flag}
 *
 * @param[in] str 原始字符串  "10\-1\0\-100"
 * @param[out] tile 解析后存储数据的瓦片
 * @return 
 */
void Map::load_tile_from_string(Tile& tile, const std::string& str)
{
    std::string str_tidy = trim_str(str);
    std::string str_value;
    std::vector<int> values;
    std::stringstream str_stream(str_tidy);

    while (std::getline(str_stream, str_value, '\\')) 
    {
        int value;
        try
        {
            value = std::stoi(str_value);
        }
        catch (const std::invalid_argument&)
        {
            value = -1;
        }
        values.push_back(value);
    }

    tile.terrian = (values.size() < 1 || values[0] < 0) ? 0 : values[0];
    tile.decoration = (values.size() < 2) ? -1 : values[1];
    tile.direction = (Tile::Direction)((values.size() < 3 || values[2] < 0) ? 0 : values[2]);
    tile.special_flag = (values.size() <= 3) ? -1 : values[3];
}

size_t Map::get_width() const
{
    if (tile_map.empty())
        return 0;
    return tile_map[0].size();
}

size_t Map::get_height() const
{
    if (tile_map.empty())
        return 0;
    return tile_map.size();
}
const TileMap& Map::get_tile_map() const
{
    return tile_map;
}
const SDL_Point& Map::get_home_point() const
{
    return idx_home;
}
const Map::SpawnerRoutePool& Map::get_spawner_route_pool() const
{
    return spawner_route_pool;
}
void Map::place_tower(const SDL_Point& idx_tile)
{
    tile_map[idx_tile.y][idx_tile.x].has_tower = true;
}
/**
 * @brief       找到地图中home位置
 *              生成怪物路径
 *
 * @param[in]
 * @param[out]
 * @return
 */
void Map::generate_map_cache()
{
    for (size_t y = 0; y < get_height(); y++)
    {
        for(size_t x = 0; x < get_width(); x++)
        {
            const Tile& tile = tile_map[y][x];
            if (tile.special_flag < 0)
                continue;
            if (tile.special_flag == 0) 
            {
                idx_home.x = (int)x;
                idx_home.y = (int)y;
            }
            else
            {
                spawner_route_pool[tile.special_flag] = Route(tile_map, SDL_Point{ (int)x,(int)y });
            }
        }
    }
}

std::string Map::trim_str(const std::string& str)
{
    size_t begin_idx = str.find_first_not_of("\t");
    if (begin_idx == std::string::npos) 
        return "";
    size_t end_idx = str.find_last_not_of("\t"); 
    size_t idx_range = end_idx - begin_idx + 1;
    return str.substr(begin_idx, idx_range);
}


