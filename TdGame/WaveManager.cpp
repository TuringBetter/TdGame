#include "WaveManager.h"
#include "EnemyManager.h"

void WaveManager::on_update(double delta)
{
	static auto instance = ConfigManager::GetInstance();
	if (instance->is_game_over)
		return;

	if (!is_wave_start)
		timer_start_wave.on_update(delta);
	else
		timer_spawn_enemy.on_update(delta);

	/*
	* 进行下一波
	* 条件：本次波场上敌人全部生成且敌人被全部消灭
	* 获得金币奖励
	*/
	if (is_spawned_last_enemy && EnemyManager::GetInstance()->check_clear()) 
	{
		CoinManager::GetInstance()->increase_coin(instance->wave_list[idx_wave].rewards);

		idx_wave++;

		if(idx_wave>=instance->wave_list.size())
		{
			instance->is_game_win = true;
			instance->is_game_over = true;
		}
		else
		{
			idx_spawn_event = 0;
			is_wave_start = true;
			is_spawned_last_enemy = false;

			const Wave& wave = instance->wave_list[idx_wave];
			timer_start_wave.set_wait_time(wave.interval);
			timer_start_wave.restart();
		}
	}
}

WaveManager::WaveManager()
{
	static const std::vector<Wave>& wave_list = ConfigManager::GetInstance()->wave_list;
	/* timer_start_wave启动第0波，生成第一个敌人 */
	timer_start_wave.set_one_shot(true);
	timer_start_wave.set_wait_time(wave_list[0].interval);
	timer_start_wave.set_callback(
		[&]()
		{
			is_wave_start = true;
			timer_spawn_enemy.set_wait_time(wave_list[idx_wave].spawn_event_list[0].interval);
			timer_spawn_enemy.restart();
		}
	);
	/* 
	* 虽然timer_spawn_enemy设置为单次触发，
	* 但是在回调函数中每次都重新设置wait_time 
	* 然后再重新触发
	*/
	timer_spawn_enemy.set_one_shot(true);
	timer_spawn_enemy.set_callback(
		[&]()
		{
			const std::vector<Wave::SpawnEvent>& spawn_event_list = wave_list[idx_wave].spawn_event_list;
			/* 此处用到了idx_spawn_event，则就要需要思考是否要更新idx_spawn_event */
			const Wave::SpawnEvent& spawn_event = spawn_event_list[idx_spawn_event];

			EnemyManager::GetInstance()->spawn_enemy(spawn_event.enemy_type, spawn_event.spawn_point);

			/* 每次更新索引就要判断是否越界，是否有效 */
			idx_spawn_event++;

			if (idx_spawn_event >= spawn_event_list.size())
			{
				is_spawned_last_enemy = true;
				return;
			}

			timer_spawn_enemy.set_wait_time(spawn_event_list[idx_spawn_event].interval);
			timer_spawn_enemy.restart();
		}
	);
}
