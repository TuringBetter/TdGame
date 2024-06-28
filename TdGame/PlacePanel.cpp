#include "PlacePanel.h"

PlacePanel::PlacePanel()
{
	const ResourceManager::TexturePool& texture_pool 
		= ResourceManager::GetInstance()->get_texture_pool(); 
	
	tex_idle = texture_pool.find(ResID::Tex_UIPlaceIdle)->second; 
	tex_hovered_top = texture_pool.find(ResID::Tex_UIPlaceHoveredTop)->second; 
	tex_hovered_left = texture_pool.find(ResID::Tex_UIPlaceHoveredLeft)->second; 
	tex_hovered_right = texture_pool.find(ResID::Tex_UIPlaceHoveredRight)->second;
	
}

void PlacePanel::on_update(SDL_Renderer* render)
{
	static auto instance = TowerManager::GetInstance();

	val_top = (int)instance->get_place_cost(TowerType::Axeman);
	val_left = (int)instance->get_place_cost(TowerType::Archer);
	val_right = (int)instance->get_place_cost(TowerType::Gunner);

	reg_top = (int)instance->get_damage_range(TowerType::Axeman) * SIZE_TILE;
	reg_left = (int)instance->get_damage_range(TowerType::Archer) * SIZE_TILE;
	reg_right = (int)instance->get_damage_range(TowerType::Gunner) * SIZE_TILE;

	Panel::on_update(render);
}

void PlacePanel::on_render(SDL_Renderer* render)
{
	if (!is_visible) return;

	int reg = 0;
	switch (hovered_target)
	{
	case Panel::HoveredTarget::Top:
		reg = reg_top;
		break;
	case Panel::HoveredTarget::Left:
		reg = reg_left;
		break;
	case Panel::HoveredTarget::Right:
		reg = reg_right;
		break;
	}

	if(reg>0)
	{
		filledCircleRGBA(render, center_pos.x, center_pos.y, reg, 
			color_region_content.r, color_region_content.g, color_region_content.b, color_region_content.a); 
		aacircleRGBA(render, center_pos.x, center_pos.y, reg,
			color_region_frame.r, color_region_frame.g, color_region_frame.b, color_region_frame.a);
	}

	Panel::on_render(render);
}

void PlacePanel::on_click_top_area()
{
	static auto instance = CoinManager::GetInstance();
	if(val_top<=instance->get_current_coin_num())
	{
		TowerManager::GetInstance()->place_tower(TowerType::Axeman, idx_tile_selected);
		instance->decrease_coin(val_top);
	}
}

void PlacePanel::on_click_left_area()
{
	static auto instance = CoinManager::GetInstance();
	if (val_left <= instance->get_current_coin_num())
	{
		TowerManager::GetInstance()->place_tower(TowerType::Archer, idx_tile_selected);
		instance->decrease_coin(val_left);
	}
}

void PlacePanel::on_click_right_area()
{
	static auto instance = CoinManager::GetInstance();
	if (val_right <= instance->get_current_coin_num())
	{
		TowerManager::GetInstance()->place_tower(TowerType::Gunner, idx_tile_selected);
		instance->decrease_coin(val_right);
	}
}
