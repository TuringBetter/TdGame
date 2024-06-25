#pragma once
#include "Manager.h"
#include "ResourceManager.h"
#include "ConfigManager.h"

class HomeManager :
    public Manager<HomeManager>
{
    friend class Manager<HomeManager>;
public:
    ~HomeManager() = default;

    double get_current_hp() const;

    void decrease_hp(double val);

private:
    double num_hp = 0;
private:
    HomeManager();
};

