#pragma once
#include "CoinManager.h"
#include "SDL2_gfxPrimitives.h"
#include "TowerManager.h"
#include "ResourceManager.h"
#include "Panel.h"
class PlacePanel:
	public Panel
{
public:
	PlacePanel();
	~PlacePanel() = default;

	void on_update(SDL_Renderer* render) override;
	void on_render(SDL_Renderer* render) override;

protected:
	void on_click_top_area() override;
	void on_click_left_area() override;
	void on_click_right_area() override;

private:
	/* ¹¥»÷·¶Î§ÑÕÉ« */
	const SDL_Color color_region_frame = { 30,80,162,175 };
	const SDL_Color color_region_content = { 0,149,217,75 };

private:
	/* ¸÷¸ö·ÀÓùËþµÄ¹¥»÷·¶Î§ */
	int reg_top = 0;
	int reg_left = 0;
	int reg_right = 0;

};  

