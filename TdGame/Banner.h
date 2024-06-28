#pragma once
#include "ConfigManager.h"
#include "ResourceManager.h"
#include "Vector2.h"
#include "SDL.h"
#include "Timer.h"
class Banner
{
public:
	Banner();
	~Banner() = default;

	void set_center_position(const Vector2& position);
	void on_update(double delta);
	void on_render(SDL_Renderer* render);
	bool check_end_display() const;
private:
	Vector2 pos_center;

	Vector2 size_foreground;
	Vector2 size_background;

	SDL_Texture* tex_foreground;
	SDL_Texture* tex_background;

	Timer timer_display;
	bool is_end_display = false;
};

