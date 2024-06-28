#pragma once
#include "Bullet.h"
#include "ResourceManager.h"
class ArrowBullet :
    public Bullet
{
public:
    ArrowBullet();
    ~ArrowBullet() = default;

    void on_collide(Enemy* enemy) override;
};

