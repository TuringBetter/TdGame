#include "Bullet.h"

void Bullet::set_velocity(const Vector2& velocity)
{
	this->velovity = velocity;
	if(can_rotated)
	{
		double randian = std::atan2(velocity.y, velocity.x);
		angle_anim_rotated = randian * 180 / M_PI;
	}
}

void Bullet::set_position(const Vector2& position)
{
	this->position = position;
}

void Bullet::set_damage(double damage)
{
	this->damage = damage;
}

const Vector2& Bullet::get_velovity() const
{
	return velovity;
}

const Vector2& Bullet::get_position() const
{
	return position;
}

double Bullet::get_damage() const
{
	return damage;
}

double Bullet::get_damage_range() const
{
	return damage_range;
}

void Bullet::disable_collide()
{
	is_collisional = false;
}

bool Bullet::can_collide()
{
	return is_collisional;
}

void Bullet::make_valid()
{
	is_valid = false;
	is_collisional = false;
}

bool Bullet::can_remove() const
{
	return !is_valid;
}

void Bullet::on_update(double delta)
{
	animation.on_update(delta);
	position += velovity * delta;

	/* 判断子弹是否超出地图 */
	static const SDL_Rect& rect_map
		= ConfigManager::GetInstance()->rect_tile_map;

	if (position.x - size.x / 2 <= rect_map.x
		|| position.x + size.x / 2 >= rect_map.x + rect_map.w
		|| position.y - size.y / 2 <= rect_map.y
		|| position.y + size.y / 2 >= rect_map.y + rect_map.h)
	{
		is_valid = false;
	}
}

void Bullet::on_render(SDL_Renderer* render)
{
	static SDL_Point point;

	point.x = (int)(position.x - size.x / 2);
	point.y = (int)(position.y - size.y / 2);

	animation.on_render(render, point, angle_anim_rotated);
}

void Bullet::on_collide(Enemy* enemy)
{
	is_valid = false;
	is_collisional = false;
}
