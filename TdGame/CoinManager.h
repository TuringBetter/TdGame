#pragma once
#include "Manager.h"
class CoinManager :
    public Manager<CoinManager>
{
    friend class Manager<CoinManager>;

public:
    ~CoinManager() = default;

    void increase_coin(double val);
    void decrease_coin(double val);

private:
    CoinManager() = default;

private:
    double num_coin = 0;
};

