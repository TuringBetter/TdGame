#pragma once
#include "Manager.h"
#include "Map.h"
#include "Wave.h"


#include <SDL.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

#include <cJSON.h>

class ConfigManager
	:
	public Manager<ConfigManager>
{
	friend class Manager<ConfigManager>;
public:
	struct WindowTemplate
	{
		std::string window_title = u8"村庄保卫战";
		int window_width = 1280;
		int window_height = 720;
	};
	struct PlayerTemplate
	{
		double speed = 3;
		double normal_attack_interval = 0.5;
		double normal_attack_damage = 0;
		double skill_interval = 10;
		double skill_damage = 1;
	};
	struct TowerTemplate
	{
		double interval[10] = { 1 };
		double damage[10] = { 25 };
		double view_range[10] = { 5 };
		double cost[10] = { 50 };
		double upgrade_cost[9] = { 75 };
	};
	struct EnemyTemplate
	{
		double hp = 100;
		double speed = 1;
		double damage = 1;
		double reward_ratio = 0.5;
		double recover_interval = 10;
		double recover_range = 0;
		double recover_intensity = 25;
	};

public:
	Map map;
	std::vector<Wave> wave_list;

	int level_archer = 0; 
	int level_axeman = 0;
	int level_gunner = 0;

	/* 
	* 游戏是否获胜
	* 房屋血量不低于0
	*/
	bool is_game_win = false;
	/* 波次是否结束 */
	bool is_game_over= false;

	/* 地图纹理位置 */
	SDL_Rect rect_tile_map = { 0 };

	WindowTemplate window_template;

	PlayerTemplate player_template; 

	TowerTemplate archer_template; 
	TowerTemplate axeman_template; 
	TowerTemplate gunner_template; 

	EnemyTemplate slim_template; 
	EnemyTemplate king_slim_template; 
	EnemyTemplate skeleton_template; 
	EnemyTemplate goblin_template; 
	EnemyTemplate goblin_priest_template; 

	const double num_initial_hp = 10; 
	const double num_initial_coin = 100; 
	
	const double num_coin_per_prop = 10;
public:
	bool load_level_config(const std::string& path);
	
	bool load_game_config(const std::string& path);

	~ConfigManager() = default;
private:

	ConfigManager()=default;

	void parse_window_template(WindowTemplate& window_template, const cJSON* json_root);
	void parse_player_template(PlayerTemplate& player_template, const cJSON* json_root);
	void parse_tower_template(TowerTemplate& tower_template, const cJSON* json_root);
	void parse_enemy_template(EnemyTemplate& enemy_template, const cJSON* json_root);

	void parse_number_array(double* ary, int max_len, const cJSON* json_root);
};

