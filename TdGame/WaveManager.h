#pragma once
#include "Manager.h"
#include "Timer.h"
#include "ConfigManager.h"

class WaveManager :
    public Manager<WaveManager>
{
    friend class Manager<WaveManager>;
public:
    ~WaveManager() = default;
    void on_update(double delta);
private:
    /* 记录当前的波次索引 */
    int idx_wave = 0;
    /* 记录当前的波次事件 */
    int idx_spawn_event = 0;
    Timer timer_start_wave;
    Timer timer_spawn_enemy;
    bool is_wave_start = false;
    bool is_spawned_last_enemy = false;
private:
    WaveManager();
};

