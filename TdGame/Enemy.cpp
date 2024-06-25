#include "Enemy.h"
/* 需要注意私有成员的初始化 */
Enemy::Enemy()
{
	timer_skill.set_one_shot(false);
	timer_skill.set_callback([&]()
		{
			on_skill_released(this);
		});

	timer_sketch.set_one_shot(true);
	timer_sketch.set_wait_time(0.075);
	timer_sketch.set_callback([&]()
		{
			is_show_sketch = false;
		});

	timer_restore_speed.set_one_shot(true);
	timer_restore_speed.set_callback([&]()
		{
			speed = max_speed;
		});
}

void Enemy::on_update(double delta)
{
	timer_skill.on_update(delta);
	timer_sketch.on_update(delta);
	timer_restore_speed.on_update(delta);

	/* 当前时刻移动的距离 */
	Vector2 move_distance = velocity * delta;
	/* 计算出当前位置距离目的地位置的距离 */
	Vector2 target_distance = position_target - position;
	/* 如果移动的距离大于到目的地的距离，则只移动目的距离，避免越过目的地 */
	position += move_distance < target_distance ? move_distance : target_distance;

	if(target_distance.approx_zero())
	{
		idx_target++;
		refresh_position_target();

		direction = (position_target - position).normalize();
	}

	velocity = direction * speed * SIZE_TILE;

	/* 处理动画 */

	/* 
	* 是否显示水平方向的动画 
	* 通过判断水平和速度方向那个速度的绝对值大
	*/
	bool is_show_x_amin = abs(velocity.x) > abs(velocity.y);

	/* 选择当前播放的动画集 */
	if(is_show_sketch)
	{
		if (is_show_x_amin)
			anim_current = velocity.x > 0 ? &anim_right_sketch : &anim_left_sketch;
		else
			anim_current = velocity.y > 0 ? &anim_down_sketch : &anim_up_sketch;
	}
	else
	{
		if (is_show_x_amin)
			anim_current = velocity.x > 0 ? &anim_right : &anim_left;
		else
			anim_current = velocity.y > 0 ? &anim_down : &anim_up;
	}

	anim_current->on_update(delta);
}

void Enemy::on_render(SDL_Renderer* render)
{
	/* 血条位置 */
	static SDL_Rect rect_hp_bar;
	static const int offset_y = 2;
	/* 血条框大小，长40px，宽8px */
	static const Vector2 size_hp_bar = { 40,8 };
	static SDL_Point point;

	static const SDL_Color color_border = { 116,185,124,255 };
	static const SDL_Color color_content = { 226,255,194,255 };

	/* 将中心点坐标转化成左上角 */
	point.x = (int)(position.x - size.x / 2);
	point.y = (int)(position.y - size.y / 2);

	/* 画人物 */
	anim_current->on_render(render, point);

	/* 画血条 */
	if(hp<max_hp)
	{
		/* 设置血条位置 */
		rect_hp_bar.x = (int)(position.x - size_hp_bar.x / 2);
		rect_hp_bar.y = (int)(position.y - size.y / 2 - size_hp_bar.y - offset_y);
		rect_hp_bar.w = (int)size_hp_bar.x * (hp / max_hp);
		rect_hp_bar.h = (int)size_hp_bar.y;

		/* 绘制血条 */
		SDL_SetRenderDrawColor(render, color_content.r, color_content.g, color_content.b, color_content.a);
		SDL_RenderFillRect(render, &rect_hp_bar);

		/* 绘制血条边框 */
		rect_hp_bar.w = (int)size_hp_bar.x;
		SDL_SetRenderDrawColor(render, color_border.r, color_border.g, color_border.b, color_border.a);
		SDL_RenderDrawRect(render, &rect_hp_bar);
		
	}
}

void Enemy::set_on_skill_released(SkillCallback on_skill_released)
{
	this->on_skill_released = on_skill_released;
}

void Enemy::increase_hp(double val)
{
	hp += val;
	if (hp > max_hp)
		hp = max_hp;
}

void Enemy::decrease_hp(double val)
{
	hp -= val;
	if (hp <=0)
	{
		hp = 0;
		is_valid = false;
	}
	/* 被攻击后闪白 */
	is_show_sketch = true;
	timer_sketch.restart();
}

void Enemy::slow_down()
{
	speed = max_speed - 0.5;
	timer_restore_speed.set_wait_time(1);
	timer_restore_speed.restart();
}

void Enemy::set_position(const Vector2& position)
{
	this->position = position;
}

void Enemy::set_route(const Route* route)
{
	this->route = route;
	refresh_position_target();
}

void Enemy::make_invalid()
{
	is_valid = false;
}

double Enemy::get_hp() const
{
	return hp;
}

const Vector2& Enemy::get_size() const
{
	return this->size;
}

const Vector2& Enemy::get_position() const
{
	return position;
}

const Vector2& Enemy::get_velocity() const
{
	return velocity;
}

double Enemy::get_damage() const
{
	return damage;
}

double Enemy::get_reward_ratio() const
{
	return reward_ratio;
}

double Enemy::get_recover_radius() const
{
	return SIZE_TILE*recover_range;
}

double Enemy::get_recover_intensity() const
{
	return recover_intensity;
}

bool Enemy::can_remove() const
{
	return !is_valid;
}

double Enemy::get_route_process() const
{
	if (route->get_idx_list().size() == 1)
		return 1;
	return (double)idx_target/(route->get_idx_list().size()-1);
}

void Enemy::refresh_position_target()
{
	static const Route::IdxList& idx_list = route->get_idx_list();

	if(idx_target<idx_list.size())
	{
		const SDL_Point& point = idx_list[idx_target];
		static const SDL_Rect& rect_tile_map = ConfigManager::GetInstance()->rect_tile_map;

		/* 
		* 下一帧目标位置 
		* x=地图左上角+目标x索引*单元格大小+单元格大小/2(取中心点)
		*/
		position_target.x = rect_tile_map.x + point.x * SIZE_TILE + SIZE_TILE / 2;
		position_target.y = rect_tile_map.y + point.y * SIZE_TILE + SIZE_TILE / 2;
	}
}
