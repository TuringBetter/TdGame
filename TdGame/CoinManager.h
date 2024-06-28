#pragma once
#include "Manager.h"
#include "CoinProp.h"


class CoinManager :
    public Manager<CoinManager>
{
    friend class Manager<CoinManager>;
public:
    typedef std::vector<CoinProp*> CoinPropList;

public:
    ~CoinManager();

    void increase_coin(double val);
    void decrease_coin(double val);

    void on_update(double delta);
    void on_render(SDL_Renderer* render);

    double get_current_coin_num() const;

    CoinPropList& get_coin_prop_list();

    void spawn_coin_prop(const Vector2& position);
private:
    CoinManager();

private:
    CoinPropList coin_prop_list;
    double num_coin = 0;
};

