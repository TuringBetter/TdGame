#include "ConfigManager.h"

bool ConfigManager::load_level_config(const std::string& path)
{
	std::ifstream file(path);
	if (!file.good()) return false;

	std::stringstream str_stream;
	str_stream << file.rdbuf();
	file.close();

	/* 
	* 解析json文件数据 
	* 注意传入的参数是C风格的字符串
	*/
	cJSON* json_root = cJSON_Parse(str_stream.str().c_str());
	/* 如果文件格式不对，一定要释放掉已经申请的内存 */
	if (!json_root) 
	{ 
		delete json_root;
		return false; 
	}

	/* 开始解析数据，root是一个数组 */
	if (cJSON_Array != json_root->type) return false;
	cJSON* json_wave = nullptr;
	cJSON_ArrayForEach(json_wave, json_root) 
	{
		if (cJSON_Object != json_wave->type)
			continue;

		wave_list.emplace_back();
		Wave& wave = wave_list.back();

		cJSON* json_wave_rewards = cJSON_GetObjectItem(json_wave, "rewards");
		if (json_wave_rewards && cJSON_Number == json_wave_rewards->type)
			wave.rewards = json_wave_rewards->valueint;
		cJSON* json_wave_interval = cJSON_GetObjectItem(json_wave, "interval");
		if (json_wave_interval && cJSON_Number == json_wave_interval->type)
			wave.interval = json_wave_interval->valuedouble;
		cJSON* json_wave_spawn_list = cJSON_GetObjectItem(json_wave, "spawn_list");
		if (json_wave_spawn_list && cJSON_Array == json_wave_spawn_list->type) 
		{
			cJSON* json_spawn_event = nullptr;
			cJSON_ArrayForEach(json_spawn_event, json_wave_spawn_list)
			{
				if (cJSON_Object != json_spawn_event->type)
					continue;
				wave.spawn_event_list.emplace_back();
				Wave::SpawnEvent& spawn_event = wave.spawn_event_list.back();

				cJSON* json_spawn_event_interval = cJSON_GetObjectItem(json_spawn_event, "interval");
				if (json_spawn_event_interval && cJSON_Number == json_spawn_event_interval->type)
					spawn_event.interval = json_spawn_event_interval->valuedouble;

				cJSON* json_spawn_event_point = cJSON_GetObjectItem(json_spawn_event, "point");
				if (json_spawn_event_point && cJSON_Number == json_spawn_event_point->type)
					spawn_event.spawn_point = json_spawn_event_point->valueint;

				cJSON* json_spawn_event_enemy_type = cJSON_GetObjectItem(json_spawn_event, "enemy");
				if (json_spawn_event_enemy_type && cJSON_String == json_spawn_event_enemy_type->type)
				{
					const std::string str_enemy_type = json_spawn_event_enemy_type->valuestring; 
					if (str_enemy_type == "slim")
						spawn_event.enemy_type = EnemyType::Slim;
					else if (str_enemy_type == "KingSlim")
						spawn_event.enemy_type = EnemyType::KingSlim;
					else if (str_enemy_type == "Skeleton")
						spawn_event.enemy_type = EnemyType::Skeleton;
					else if (str_enemy_type == "Goblin")
						spawn_event.enemy_type = EnemyType::Goblin;
					else if (str_enemy_type == "GoblinPriest")
						spawn_event.enemy_type = EnemyType::GoblinPriest;
				}
			}

			if (wave.spawn_event_list.empty())
				wave_list.pop_back();
		}
	}

	cJSON_Delete(json_root);

	if (wave_list.empty())
		return false;

	return true;
}

bool ConfigManager::load_game_config(const std::string& path)
{
	std::ifstream file(path);
	if (!file.good()) return false;

	std::stringstream str_stream;
	str_stream << file.rdbuf();
	file.close();

	cJSON* json_root = cJSON_Parse(str_stream.str().c_str());
	if (!json_root)
	{
		delete json_root;
		return false;
	}

	cJSON* json_window = cJSON_GetObjectItem(json_root, "window");
	cJSON* json_player = cJSON_GetObjectItem(json_root, "player");
	cJSON* json_tower  = cJSON_GetObjectItem(json_root, "tower");
	cJSON* json_enemy  = cJSON_GetObjectItem(json_root, "enemy");

	if (!json_player || cJSON_Object != json_player->type)
	{
		cJSON_Delete(json_root);
		return false;
	}

	if(!json_window||cJSON_Object!= json_window->type)
	{
		cJSON_Delete(json_root);
		return false;
	}

	if (!json_tower || cJSON_Object != json_tower->type)
	{
		cJSON_Delete(json_root);
		return false;
	}

	if (!json_enemy || cJSON_Object != json_enemy->type)
	{
		cJSON_Delete(json_root);
		return false;
	}

	parse_window_template(window_template, json_window);
	parse_player_template(player_template, json_player);

	parse_tower_template(archer_template, cJSON_GetObjectItem(json_tower, "archer"));
	parse_tower_template(axeman_template, cJSON_GetObjectItem(json_tower, "axeman"));
	parse_tower_template(gunner_template, cJSON_GetObjectItem(json_tower, "gunner"));


	parse_enemy_template(slim_template, cJSON_GetObjectItem(json_enemy, "slim"));
	parse_enemy_template(king_slim_template, cJSON_GetObjectItem(json_enemy, "king_slim"));
	parse_enemy_template(skeleton_template, cJSON_GetObjectItem(json_enemy, "skeleton"));
	parse_enemy_template(goblin_template, cJSON_GetObjectItem(json_enemy, "goblin"));
	parse_enemy_template(goblin_priest_template, cJSON_GetObjectItem(json_enemy, "goblin_priest"));


	cJSON_Delete(json_root);
	return true;
}


void ConfigManager::parse_window_template(WindowTemplate& window_template, const cJSON* json_root)
{
	if (!json_root || json_root->type != cJSON_Object) return;

	cJSON* json_window_title = cJSON_GetObjectItem(json_root, "window_title"); 
	cJSON* json_window_width = cJSON_GetObjectItem(json_root, "window_width"); 
	cJSON* json_window_height = cJSON_GetObjectItem(json_root, "window_height");
	
	if(json_window_title && json_window_title->type == cJSON_String)
		window_template.window_title = json_window_title->valuestring;
	if(json_window_width && json_window_width->type == cJSON_Number)
		window_template.window_width =json_window_width->valueint;
	if(json_window_height && json_window_height->type == cJSON_Number)
		window_template.window_height =json_window_height->valueint;
}

void ConfigManager::parse_player_template(PlayerTemplate& player_template, const cJSON* json_root)
{
	if (!json_root||json_root->type != cJSON_Object) return; 

	cJSON* json_speed = cJSON_GetObjectItem(json_root, "speed"); 
	cJSON* json_normal_attack_interval = cJSON_GetObjectItem(json_root, "normal_attack_interval"); 
	cJSON* json_normal_attack_damage = cJSON_GetObjectItem(json_root, "normal_attack_damage"); 
	cJSON* json_skill_interval = cJSON_GetObjectItem(json_root, "skill_interval"); 
	cJSON* json_skill_damage = cJSON_GetObjectItem(json_root, "skill_damage");

	if(json_speed && json_speed->type == cJSON_Number)
		player_template.speed=json_speed->valuedouble;
	if(json_normal_attack_interval && json_normal_attack_interval->type == cJSON_Number)
		player_template.normal_attack_interval =json_normal_attack_interval->valuedouble;
	if(json_normal_attack_damage && json_normal_attack_damage->type == cJSON_Number)
		player_template.normal_attack_damage=json_normal_attack_damage->valuedouble;
	if (json_skill_interval && json_skill_interval->type == cJSON_Number)
		player_template.skill_interval =json_skill_interval->valuedouble;
	if (json_skill_damage && json_skill_damage->type == cJSON_Number)
		player_template.skill_damage=json_skill_damage->valuedouble;
}

void ConfigManager::parse_tower_template(TowerTemplate& tower_template, const cJSON* json_root)
{
	if (!json_root || json_root->type != cJSON_Object) return;

	cJSON* json_interval = cJSON_GetObjectItem(json_root, "interval"); 
	cJSON* json_damage = cJSON_GetObjectItem(json_root, "damage");
	cJSON* json_view_range = cJSON_GetObjectItem(json_root,"view_range");
	cJSON* json_cost = cJSON_GetObjectItem(json_root,"cost");
	cJSON* json_upgrade_cost = cJSON_GetObjectItem(json_root,"upgrade_cost");

	parse_number_array(tower_template.interval, 10, json_interval);
	parse_number_array(tower_template.damage, 10, json_damage);
	parse_number_array(tower_template.view_range, 10, json_view_range);
	parse_number_array(tower_template.cost, 10, json_cost);
	parse_number_array(tower_template.upgrade_cost, 9, json_upgrade_cost);
}

void ConfigManager::parse_enemy_template(EnemyTemplate& enemy_template, const cJSON* json_root)
{
	if (!json_root || json_root->type== cJSON_Object) return; 
	cJSON* json_hp = cJSON_GetObjectItem(json_root, "hp"); 
	cJSON* json_speed = cJSON_GetObjectItem(json_root, "speed"); 
	cJSON* json_damage = cJSON_GetObjectItem(json_root, "damage");
	cJSON* json_reward_ratio = cJSON_GetObjectItem(json_root,"reward_ratio");
	cJSON* json_recover_interval=cJSON_GetObjectItem(json_root,"recover_interval");
	cJSON* json_recover_range = cJSON_GetObjectItem(json_root,"recover_range");
	cJSON* json_recover_intensity=cJSON_GetObjectItem(json_root,"recover_intensity");
	
	if (json_hp && json_hp->type == cJSON_Number)
		enemy_template.hp =json_hp->valuedouble;
	if (json_speed && json_speed->type == cJSON_Number)
		enemy_template.speed=json_speed->valuedouble;
	if (json_damage && json_damage->type == cJSON_Number)
		enemy_template.damage=json_damage->valuedouble;
	if (json_reward_ratio && json_reward_ratio->type == cJSON_Number)
		enemy_template.reward_ratio = json_reward_ratio->valuedouble;
	if (json_recover_interval && json_recover_interval->type == cJSON_Number)
		enemy_template.recover_interval =json_recover_interval->valuedouble;
	if (json_recover_range && json_recover_range->type == cJSON_Number)
		enemy_template.recover_range =json_recover_range->valuedouble;
	if (json_recover_intensity && json_recover_intensity->type == cJSON_Number)
		enemy_template.recover_intensity = json_recover_intensity-> valuedouble;

}

/**
 * @brief		解释json 数字数字，格式为
 *				"damage":	[10, 12, 14, 16, 18, 20, 22, 24, 25, 25]
 *
 * @param[in]   max_len:数组长度
 * @param[in]   json_root:json数据
 * @param[out]  ary:数据
 * @return
 */

void ConfigManager::parse_number_array(double* ary, int max_len, const cJSON* json_root)
{
	if (!json_root || json_root->type != cJSON_Array) return;

	int idx = -1;
	cJSON* json_element = nullptr;
	cJSON_ArrayForEach(json_element, json_root) 
	{
		idx++;
		if (json_element->type != cJSON_Number || idx >= max_len)
			continue;
		ary[idx] = json_element->valuedouble;
	}

}
