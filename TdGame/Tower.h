#pragma once
#include "Facing.h"
#include "Enemy.h"
#include "EnemyManager.h"
#include "BulletManager.h"
#include "TowerType.h"
#include "BulletType.h"
#include "Animation.h"
#include "Vector2.h"
class Tower
{
public:
	Tower();
	~Tower() = default;

	void set_position(const Vector2& position);

	const Vector2& get_position() const;
	const Vector2& get_size() const;
	
	void on_update(double delta);
	void on_render(SDL_Renderer* render);
	void on_fire();

	Enemy* find_target_enemy();

protected:
	Vector2 size;

	Animation anim_idle_up;
	Animation anim_idle_down;
	Animation anim_idle_left;
	Animation anim_idle_right;
	Animation anim_fire_up;
	Animation anim_fire_down;
	Animation anim_fire_left;
	Animation anim_fire_right;

	TowerType tower_type = TowerType::Archer;

	double fire_speed = 0;
	BulletType bullet_type = BulletType::Arrow;
private:
	Timer timer_fire;
	Vector2 position;
	bool can_fire = true;

	Facing facing = Facing::Right;

	Animation* anim_current = &anim_idle_right;

private:
	void update_idle_animation();
	void update_fire_animation();
};
