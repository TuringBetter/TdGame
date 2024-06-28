#pragma once
#include "Bullet.h"
#include "ResourceManager.h"
class AxeBullet :
    public Bullet
{
public:
    AxeBullet();
    ~AxeBullet() = default;

    void on_collide(Enemy* enemy) override;
};

