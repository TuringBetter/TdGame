#include "UpgradePanel.h"

UpgradePanel::UpgradePanel()
{
	const ResourceManager::TexturePool& texture_pool 
		= ResourceManager::GetInstance()->get_texture_pool(); 
	
	tex_idle = texture_pool.find(ResID::Tex_UIUpgradeIdle)->second; 
	tex_hovered_top = texture_pool.find(ResID::Tex_UIUpgradeHoveredTop)->second; 
	tex_hovered_left = texture_pool.find(ResID::Tex_UIUpgradeHoveredLeft)->second; 
	tex_hovered_right = texture_pool.find(ResID::Tex_UIUpgradeHoveredRight)->second;
}

void UpgradePanel::on_update(SDL_Renderer* render)
{
	static auto instance = TowerManager::GetInstance();

	val_top = (int)instance->get_upgrate_cost(TowerType::Axeman);
	val_left = (int)instance->get_upgrate_cost(TowerType::Archer);
	val_right = (int)instance->get_upgrate_cost(TowerType::Gunner);

	Panel::on_update(render);
}

void UpgradePanel::on_click_top_area()
{
	static auto instance = CoinManager::GetInstance();
	if (val_top>0&&val_top <= instance->get_current_coin_num())
	{
		TowerManager::GetInstance()->upgrate_tower(TowerType::Archer);
		instance->decrease_coin(val_top);
	}
}

void UpgradePanel::on_click_left_area()
{
	static auto instance = CoinManager::GetInstance();
	if (val_left > 0 && val_left <= instance->get_current_coin_num())
	{
		TowerManager::GetInstance()->upgrate_tower(TowerType::Axeman);
		instance->decrease_coin(val_left);
	}
}

void UpgradePanel::on_click_right_area()
{
	static auto instance = CoinManager::GetInstance();
	if (val_right > 0 && val_right <= instance->get_current_coin_num())
	{
		TowerManager::GetInstance()->upgrate_tower(TowerType::Gunner);
		instance->decrease_coin(val_right);
	}
}
