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
    /* ��¼��ǰ�Ĳ������� */
    int idx_wave = 0;
    /* ��¼��ǰ�Ĳ����¼� */
    int idx_spawn_event = 0;
    Timer timer_start_wave;
    Timer timer_spawn_enemy;
    bool is_wave_start = false;
    bool is_spawned_last_enemy = false;
private:
    WaveManager();
};

