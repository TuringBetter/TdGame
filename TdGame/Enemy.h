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
	* ���ⲿ���ù�����Ч�ķ��� 
	* ����������ﵽ���ݻ���
	*/
	void make_invalid();
	double get_hp() const;
	/*
	* ��ײ�������õ���
	* ��ȡ����ߴ硢λ�á��ٶȡ��˺�
	*/
	const Vector2& get_size() const;
	const Vector2& get_position() const;
	const Vector2& get_velocity() const;
	double get_damage() const;
	double get_reward_ratio() const;
	double get_recover_radius() const;
	double get_recover_intensity() const;	
	bool can_remove() const;
	/* ���������� */
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
	/* ÿ���ƶ����ٵ�Ԫ�� */
	double speed = 0;
	double max_speed = 0;
	double damage = 0;
	double reward_ratio = 0;
	double recover_interval = 0;
	double recover_range = 0;
	double recover_intensity = 0;

private:
	/* ��ǰʱ�������λ�á��ٶȺͷ��� */
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
	/* ��ǰ֡�ߵ���Ŀ��·���б��е���һ������ */
	int idx_target = 0;
	/* ��ǰ֡Ҫ�ƶ���Ŀ�ĵص�λ�� */
	Vector2 position_target;

private:
	void refresh_position_target();
};

