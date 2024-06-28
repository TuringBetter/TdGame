#pragma once
#include "Manager.h"
#include "TowerType.h"
#include "Tower.h"
#include "ArcherTower.h"
#include "AxemanTower.h"
#include "GunnerTower.h"
#include "ResourceManager.h"

#include <vector>
class TowerManager :
    public Manager<TowerManager>
{
    friend class Manager<TowerManager>;
public:
    ~TowerManager() = default;

    void on_update(double delta);
    void on_render(SDL_Renderer* render);
    double get_place_cost(TowerType type);
    double get_upgrate_cost(TowerType type);
    double get_damage_range(TowerType type);

    void place_tower(TowerType type, const SDL_Point& idx);
    void upgrate_tower(TowerType type);

private:
    std::vector<Tower*> tower_list;
private:
    TowerManager() = default;
};

