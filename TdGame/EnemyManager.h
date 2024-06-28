#pragma once
#include "Manager.h"
#include "Enemy.h"
#include "ConfigManager.h"
#include "HomeManager.h"
#include "CoinManager.h"
#include "BulletManager.h"

#include <vector>
#include <SDL.h>

class EnemyManager:
	public Manager<EnemyManager>
{
	friend class Manager<EnemyManager>;
public:
	typedef std::vector<Enemy*> EnemyList;

public:
	~EnemyManager();

	void on_update(double delta);
	void on_render(SDL_Renderer* render);

	void spawn_enemy(EnemyType type, int idx_spawn_point);

	bool check_clear();

	EnemyList& get_enemy_list();
private:
	EnemyList enemy_list;

private:
	EnemyManager() = default;

	void process_home_collision();
	void process_bullet_collision();
	void remove_invalid_enemy();
	void try_spawn_coin_prop(const Vector2& position,double ratio);
};

