#pragma once
#include "Manager.h"

#include <SDL.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

class ConfigManager:
	public Manager<ConfigManager>
{
	friend class Manager<ConfigManager>;
public:
	struct BasicTemplate
	{
		std::string window_title = u8"´å×¯±£ÎÀÕ½";
		int window_width = 1280;
		int window_height = 720;
	};
	struct PlayerTemplatedouble
	{
		double speed = 3;
		double normal_attack_interval = 0.5;
		double normal_attack_damage = 0;
		double skill_interval = 10;
		double skill_damage = 1;
	};
};

