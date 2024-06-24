#pragma once
#include "EnemyType.h"
#include <vector>
struct Wave
{
	struct SpawnEvent	
	{
		double interval = 0;
		int spawn_point = 1;
		EnemyType enemy_type = EnemyType::Slim;
	};
	int rewards = 0;
	double interval = 0;
	std::vector<SpawnEvent> spawn_event_list;
};

