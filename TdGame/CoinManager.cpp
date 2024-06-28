#include "CoinManager.h"

CoinManager::~CoinManager()
{
	for (CoinProp* coin_prop : coin_prop_list)
		delete coin_prop;
}

void CoinManager::increase_coin(double val)
{
	num_coin += val;
}

void CoinManager::decrease_coin(double val)
{
	num_coin -= val;
	if (num_coin <= 0)
		num_coin = 0;
}

void CoinManager::on_update(double delta)
{
	for (CoinProp* coin_prop : coin_prop_list)
		coin_prop->on_update(delta);

	coin_prop_list.erase(std::remove_if(coin_prop_list.begin(), coin_prop_list.end(),
		[](const CoinProp* coin_prop)
		{
			bool deletable = coin_prop->can_remove();
			if (deletable) delete coin_prop;
			return deletable;
		}), coin_prop_list.end());
}

void CoinManager::on_render(SDL_Renderer* render)
{
	for (CoinProp* coin_prop : coin_prop_list)
		coin_prop->on_render(render);
}

double CoinManager::get_current_coin_num() const
{
	return num_coin;
}

CoinManager::CoinPropList& CoinManager::get_coin_prop_list()
{
	return coin_prop_list;
	// TODO: 在此处插入 return 语句
}

void CoinManager::spawn_coin_prop(const Vector2& position)
{
	CoinProp* coin_prop = new CoinProp();
	coin_prop->set_position(position);

	coin_prop_list.push_back(coin_prop);
}

CoinManager::CoinManager()
{
	num_coin = ConfigManager::GetInstance()->num_initial_coin;
}
