#pragma once
#include "CoinManager.h"
#include "ConfigManager.h"
#include "TowerManager.h"
#include "ResourceManager.h"
#include "Panel.h"
class UpgradePanel :
    public Panel
{
public:
    UpgradePanel();
    ~UpgradePanel() = default;

    void on_update(SDL_Renderer* render)override;
protected:
    void on_click_top_area() override;
    void on_click_left_area() override;
    void on_click_right_area() override;
};

