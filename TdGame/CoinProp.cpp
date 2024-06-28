#include "CoinProp.h"

CoinProp::CoinProp()
{
	timer_jump.set_one_shot(true);
	timer_jump.set_wait_time(interval_jump);
	timer_jump.set_callback(
		[&]()
		{
			is_jumping = false;
		}
	);

	timer_disappear.set_one_shot(true);
	timer_disappear.set_wait_time(interval_disappear);
	timer_disappear.set_callback(
		[&]()
		{
			is_valid = false;
		}
	);

	velocity.x = (rand() % 2 ? 1 : -1) * 2 * SIZE_TILE;
	velocity.y = -3 * SIZE_TILE;

}

void CoinProp::set_position(const Vector2& position)
{
	this->position = position;
}

const Vector2& CoinProp::get_position() const
{
	return position;
	// TODO: 在此处插入 return 语句
}

const Vector2& CoinProp::get_size() const
{
	return size;
	// TODO: 在此处插入 return 语句
}

void CoinProp::make_invalid()
{
	is_valid = false;
}

bool CoinProp::can_remove() const
{
	return !is_valid;
}

void CoinProp::on_render(SDL_Renderer* render)
{
	static SDL_Rect rect = { 0,0,(int)size.x,(int)size.y };
	static SDL_Texture* tex_coin = ResourceManager::GetInstance()
		->get_texture_pool().find(ResID::Tex_Coin)->second;

	rect.x = (int)(position.x - size.x / 2);
	rect.y = (int)(position.y - size.y / 2);

	SDL_RenderCopy(render, tex_coin, nullptr, &rect);
}

void CoinProp::on_update(double delta)
{
	timer_jump.on_update(delta);
	timer_disappear.on_update(delta);

	if(is_jumping)
	{
		velocity.y += gravity * delta;
	}
	else
	{
		velocity.x = 0;
		velocity.y = sin(SDL_GetTicks64() / 1000 * 4) * 30;
	}

	position += velocity * delta;
}
