#pragma once
#include "Tile.h"
#include "Vector2.h"
#include "Timer.h"
#include "ConfigManager.h"
#include "ResourceManager.h"
#include <SDL.h>
class CoinProp
{

public:
	CoinProp();
	~CoinProp() = default;

	void set_position(const Vector2& position);
	const Vector2& get_position() const;
	const Vector2& get_size() const;

	void make_invalid();
	bool can_remove()const;

	void on_render(SDL_Renderer* render);
	void on_update(double delta);

private:
	Vector2 position;
	/* 金币刚爆出来的速度 */
	Vector2 velocity;

	Timer timer_jump;
	Timer timer_disappear;

	bool is_valid = true;
	bool is_jumping = true;

	double gravity = 490;
	double interval_jump = 0.75;
	Vector2 size = { 16,16 };
	double interval_disappear = 10;
};

