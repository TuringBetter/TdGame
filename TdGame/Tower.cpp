#include "Tower.h"

Tower::Tower()
{
	/* 
	* timer_fire 设计思路
	* 只修改can_fire值
	* 具体的发射和渲染交由update和render
	*/
	timer_fire.set_one_shot(true);
	timer_fire.set_callback(
		[&]()
		{
			can_fire = true;
		}
	);

	anim_idle_down.set_loop(true);
	anim_idle_down.set_interval(0.2);
	anim_idle_up.set_loop(true);
	anim_idle_up.set_interval(0.2);
	anim_idle_right.set_loop(true);
	anim_idle_right.set_interval(0.2);
	anim_idle_left.set_loop(true);
	anim_idle_left.set_interval(0.2);

	/* 开火结束了，更新防御塔朝向 */
	anim_fire_up.set_loop(true);
	anim_fire_up.set_interval(0.2);
	anim_fire_up.set_callback(
		[&]()
		{
			update_idle_animation();
		}
	);
	anim_fire_down.set_loop(true);
	anim_fire_down.set_interval(0.2);
	anim_fire_down.set_callback(
		[&]()
		{
			update_idle_animation();
		}
	);
	anim_fire_right.set_loop(true);
	anim_fire_right.set_interval(0.2);
	anim_fire_right.set_callback(
		[&]()
		{
			update_idle_animation();
		}
	);
	anim_fire_left.set_loop(true);
	anim_fire_left.set_interval(0.2);
	anim_fire_left.set_callback(
		[&]()
		{
			update_idle_animation();
		}
	);


}

void Tower::set_position(const Vector2& position)
{
	this->position = position;
}

const Vector2& Tower::get_position() const
{
	return position;
	// TODO: 在此处插入 return 语句
}

const Vector2& Tower::get_size() const
{
	return size;
	// TODO: 在此处插入 return 语句
}

void Tower::on_update(double delta)
{
	timer_fire.on_update(delta);
	anim_current->on_update(delta);

	if (can_fire)
		on_fire();
}

void Tower::on_render(SDL_Renderer* render)
{
	static SDL_Point point;

	point.x = (int)(position.x - size.x / 2);
	point.y = (int)(position.y - size.y / 2);

	anim_current->on_render(render, point);
}

void Tower::on_fire()
{
	/* 找到视野范围内的敌人 */
	Enemy* target_enemy = find_target_enemy();
	if (!target_enemy)
		return;

	can_fire = false;
	/* 更新开火定时器 */
	/* 获取到子弹类型和伤害 */
	static auto instance = ConfigManager::GetInstance();
	static const ResourceManager::SoundPool& sound_pool = ResourceManager::GetInstance()->get_sound_pool();

	double interval = 0;
	double damage = 0;
	switch (tower_type)
	{
	case Archer:
		interval = instance->archer_template.interval[instance->level_archer];
		damage= instance->archer_template.damage[instance->level_archer];
		switch (rand()%2)
		{
		case 0:
			Mix_PlayChannel(-1, sound_pool.find(ResID::Sound_ArrowFire_1)->second, 0);
			break;
		case 1:
			Mix_PlayChannel(-1, sound_pool.find(ResID::Sound_ArrowFire_2)->second, 0);
			break;
		}
		break;
	case Axeman:
		interval = instance->axeman_template.interval[instance->level_axeman];
		damage = instance->axeman_template.damage[instance->level_axeman];
		Mix_PlayChannel(-1, sound_pool.find(ResID::Sound_AxeFire)->second, 0);
		break;
	case Gunner:
		interval = instance->gunner_template.interval[instance->level_gunner];
		damage = instance->gunner_template.damage[instance->level_gunner];
		Mix_PlayChannel(-1, sound_pool.find(ResID::Sound_ShellFire)->second, 0);
		break;
	}

	timer_fire.set_wait_time(interval);
	timer_fire.restart();

	/* 子弹生成逻辑 */
	Vector2 direction = target_enemy->get_position() - position;
	BulletManager::GetInstance()->fire_bullet(bullet_type, position, direction.normalize() * fire_speed * SIZE_TILE, damage);

	/* 根据速度选择当前塔的朝向 */

	bool is_show_x_anim = abs(direction.x) >= abs(direction.y);
	if (is_show_x_anim)
		facing = direction.x > 0 ? Facing::Right : Facing::Left;
	else
		facing = direction.y > 0 ? Facing::Down : Facing::Up;

	update_fire_animation();
	anim_current->reset();
}

/*
* 1、先找视野内的敌人
* 2、再找离塔最近的敌人
*/
Enemy* Tower::find_target_enemy()
{
	double max_process = -1;
	double view_range = 0;
	Enemy* enemy_target = nullptr;

	static auto instance = ConfigManager::GetInstance();

	switch (tower_type)
	{
	case Archer:
		view_range = instance->archer_template.view_range[instance->level_archer];
		break;
	case Axeman:
		view_range = instance->axeman_template.view_range[instance->level_axeman];
		break;
	case Gunner:
		view_range = instance->gunner_template.view_range[instance->level_gunner];
		break;
	}

	EnemyManager::EnemyList& enemy_list = EnemyManager::GetInstance()->get_enemy_list();
	for(Enemy* enemy:enemy_list)
	{
		if((enemy->get_position()-position).length()<=view_range*SIZE_TILE)
		{
			double new_process = enemy->get_route_process();
			if(new_process>max_process)
			{
				max_process = new_process;
				enemy_target = enemy;
			}
		}
	}

	return enemy_target;
}

void Tower::update_idle_animation()
{
	switch (facing)
	{
	case Left:
		anim_current = &anim_idle_left;
		break;
	case Right:
		anim_current = &anim_idle_right;
		break;
	case Up:
		anim_current = &anim_idle_up;
		break;
	case Down:
		anim_current = &anim_idle_down;
		break;
	default:
		anim_current = &anim_idle_left;
		break;
	}
}

void Tower::update_fire_animation()
{
	switch (facing)
	{
	case Left:
		anim_current = &anim_fire_left;
		break;
	case Right:
		anim_current = &anim_fire_right;
		break;
	case Up:
		anim_current = &anim_fire_up;
		break;
	case Down:
		anim_current = &anim_fire_down;
		break;
	default:
		anim_current = &anim_fire_left;
		break;
	}
}
