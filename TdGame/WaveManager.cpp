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
	* ������һ��
	* ���������β����ϵ���ȫ�������ҵ��˱�ȫ������
	* ��ý�ҽ���
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
	/* timer_start_wave������0�������ɵ�һ������ */
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
	* ��Ȼtimer_spawn_enemy����Ϊ���δ�����
	* �����ڻص�������ÿ�ζ���������wait_time 
	* Ȼ�������´���
	*/
	timer_spawn_enemy.set_one_shot(true);
	timer_spawn_enemy.set_callback(
		[&]()
		{
			const std::vector<Wave::SpawnEvent>& spawn_event_list = wave_list[idx_wave].spawn_event_list;
			/* �˴��õ���idx_spawn_event�����Ҫ��Ҫ˼���Ƿ�Ҫ����idx_spawn_event */
			const Wave::SpawnEvent& spawn_event = spawn_event_list[idx_spawn_event];

			EnemyManager::GetInstance()->spawn_enemy(spawn_event.enemy_type, spawn_event.spawn_point);

			/* ÿ�θ���������Ҫ�ж��Ƿ�Խ�磬�Ƿ���Ч */
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
