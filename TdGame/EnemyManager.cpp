#include "EnemyManager.h"
#include "GoblinEnemy.h"
#include "GoblinPriestEnemy.h"
#include "SlimEnemy.h"
#include "KingSlimEnemy.h"
#include "SkeletonEnemy.h"

EnemyManager::~EnemyManager()
{
	for (Enemy* enemy : enemy_list)
		delete enemy;
}

void EnemyManager::on_update(double delta)
{
	for (Enemy* enemy : enemy_list)
		enemy->on_update(delta);

	/* 处理碰撞 */
	process_home_collision();
	process_bullet_collision();

	/* 移除无效敌人 */
	remove_invalid_enemy();
}

void EnemyManager::on_render(SDL_Renderer* render)
{
	for (Enemy* enemy : enemy_list)
		enemy->on_render(render);
}


/**
 * @brief      生成敌人
 *             只传入枚举，则外部调用者不需要关心类的new、初始化
 * @param[in]  type 生成敌人的类型
 * @param[out] idx_spawn_point 敌人的出生点 0~9
 * @return
 */
void EnemyManager::spawn_enemy(EnemyType type, int idx_spawn_point)
{
	static Vector2 position;
	static const SDL_Rect& rect_tile_map = ConfigManager::GetInstance()->rect_tile_map;
	static const Map::SpawnerRoutePool& spawner_route_pool
		= ConfigManager::GetInstance()->map.get_spawner_route_pool();

	/* 生成敌人 */
	const auto& itor = spawner_route_pool.find(idx_spawn_point);
	if (itor == spawner_route_pool.end())
		return;

	Enemy* enemy = nullptr;
	switch (type)
	{
	case EnemyType::Slim:
		enemy = new SlimEnemy();
		break;
	case EnemyType::KingSlim:
		enemy = new KingSlimEnemy();
		break;
	case EnemyType::Skeleton:
		enemy = new SkeletonEnemy();
		break;
	case EnemyType::Goblin:
		enemy = new GoblinEnemy();
		break;
	case EnemyType::GoblinPriest:
		enemy = new GoblinPriestEnemy();
		break;
	default:
		enemy = new SlimEnemy();
		break;
	}

	/* 设置敌人技能 */
	enemy->set_on_skill_released(
		[&](Enemy* enemy_src)
		{
			/* 获取恢复技能的恢复范围 */
			double recover_radius = enemy_src->get_recover_radius();
			/* 无恢复范围则返回 */
			if (recover_radius <= 0)
				return;
			/* 遍历其他所有敌人，看其是否在恢复范围之内 */
			const Vector2 pos_src = enemy_src->get_position();
			for (Enemy* enemy_dst : enemy_list)
			{
				const Vector2& pos_dst = enemy_dst->get_position();
				double distance = (pos_dst - pos_src).length();
				if (distance <= recover_radius)
					enemy_dst->increase_hp(enemy_src->get_recover_intensity());
			}
		});

	/* 寻路 */
	/* 先找到出生点对应的路径 */
	const Route::IdxList& idx_list = itor->second.get_idx_list();
	position.x = rect_tile_map.x + idx_list[0].x * SIZE_TILE + SIZE_TILE / 2;
	position.y = rect_tile_map.y + idx_list[0].y * SIZE_TILE + SIZE_TILE / 2;

	enemy->set_position(position);
	enemy->set_route(&itor->second);

	enemy_list.push_back(enemy);
}

bool EnemyManager::check_clear()
{
	return enemy_list.empty();
}

void EnemyManager::process_home_collision()
{
	static const SDL_Point& idx_home = ConfigManager::GetInstance()->map.get_home_point();
	static const SDL_Rect& rect_tile_map = ConfigManager::GetInstance()->rect_tile_map;
	static const Vector2& position_home_tile =
	{
		(double)(rect_tile_map.x + idx_home.x * SIZE_TILE),
		(double)(rect_tile_map.y + idx_home.y * SIZE_TILE)
	};

	for (Enemy* enemy : enemy_list)
	{
		if (enemy->can_remove()) continue;

		const Vector2& position = enemy->get_position();

		if(position.x>=position_home_tile.x
		   && position.y >= position_home_tile.y
		   && position.x <= position_home_tile.x+SIZE_TILE
		   && position.y <= position_home_tile.y+SIZE_TILE)
		{
			enemy->make_invalid();

			HomeManager::GetInstance()->decrease_hp(enemy->get_damage());
		}
	}
}

void EnemyManager::process_bullet_collision()
{
}

void EnemyManager::remove_invalid_enemy()
{
	enemy_list.erase(std::remove_if(enemy_list.begin(), enemy_list.end(),
		[](const Enemy* enemy)
		{
			bool deletable = enemy->can_remove();
			if (deletable) delete enemy;
			return deletable;
		}), enemy_list.end());
}
