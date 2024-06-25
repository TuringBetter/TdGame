#pragma once

#include "Vector2.h"
#include "Route.h"
#include "Timer.h"
#include "ConfigManager.h"
#include "Animation.h"

#include <functional>
class Enemy
{
public:
	typedef std::function<void(Enemy* enemy)> SkillCallback;
public:
	Enemy();
	~Enemy() = default;

	void on_update(double delta);
	void on_render(SDL_Renderer* render);
	void set_on_skill_released(SkillCallback on_skill_released);
	void increase_hp(double val);
	void decrease_hp(double val);
	void slow_down();
	void set_position(const Vector2& position);
	void set_route(const Route* route);
	/* 
	* 从外部设置怪物无效的方法 
	* 例如怪物最后达到房屋基地
	*/
	void make_invalid();
	double get_hp() const;
	/*
	* 碰撞检测可能用到的
	* 获取怪物尺寸、位置、速度、伤害
	*/
	const Vector2& get_size() const;
	const Vector2& get_position() const;
	const Vector2& get_velocity() const;
	double get_damage() const;
	double get_reward_ratio() const;
	double get_recover_radius() const;
	double get_recover_intensity() const;	
	bool can_remove() const;
	/* 防御塔索敌 */
	double get_route_process() const;

protected:
	Vector2 size;

	Timer timer_skill;

	Animation anim_up;
	Animation anim_down;
	Animation anim_left;
	Animation anim_right;
	Animation anim_up_sketch;
	Animation anim_down_sketch;
	Animation anim_left_sketch;
	Animation anim_right_sketch;

	double hp = 0;
	double max_hp = 0;
	/* 每秒移动多少单元格 */
	double speed = 0;
	double max_speed = 0;
	double damage = 0;
	double reward_ratio = 0;
	double recover_interval = 0;
	double recover_range = 0;
	double recover_intensity = 0;

private:
	/* 当前时刻人物的位置、速度和方向 */
	Vector2 position;
	Vector2 velocity;
	Vector2 direction;

	bool is_valid = true;

	Timer timer_sketch;
	bool is_show_sketch = false;

	Animation* anim_current = nullptr;

	SkillCallback on_skill_released;
	
	Timer timer_restore_speed;
	const Route* route = nullptr;
	/* 当前帧走到了目标路径列表中的哪一个格子 */
	int idx_target = 0;
	/* 当前帧要移动的目的地的位置 */
	Vector2 position_target;

private:
	void refresh_position_target();
};

