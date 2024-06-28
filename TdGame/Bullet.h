#pragma once
#include "Vector2.h"
#include "Enemy.h"
#include "Animation.h"
#include "ConfigManager.h"

class Bullet
{
public:
	Bullet() = default;
	~Bullet() = default;

	void set_velocity(const Vector2& velocity);
	void set_position(const Vector2& position);
	void set_damage(double damage);

	const Vector2& get_velovity() const;
	const Vector2& get_position() const;
	double get_damage() const;
	double get_damage_range() const;

	void disable_collide();

	bool can_collide();
	void make_valid();

	bool can_remove() const;

	virtual void on_update(double delta);

	virtual void on_render(SDL_Renderer* render);

	virtual void on_collide(Enemy* enemy);
protected:
	Vector2 size;
	Vector2 position;
	Vector2 velovity;

	Animation animation;
	bool can_rotated = false;

	double damage = 0;
	double damage_range = 0;

private:
	/* 
	* 子弹分为三个阶段		is_valid		is_collisional
	* 1、飞行阶段			true			true
	* 2、爆炸阶段			true			false
	* 3、爆炸后的移除阶段		false			false
	*/
	bool is_valid = true;
	bool is_collisional = true;
	/* 设置子弹角度是一个统一的接口 */
	double angle_anim_rotated = 0;

};

