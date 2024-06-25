#include "CoinManager.h"

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
