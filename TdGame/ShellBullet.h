#pragma once
#include "Bullet.h"
#include "ResourceManager.h"
class ShellBullet :
    public Bullet
{
public:
    ShellBullet();
    ~ShellBullet() = default;

    /*
    * 如果还没爆炸，则按照父类更新
    * 否则更新子类的爆炸逻辑
    */
    void on_render(SDL_Renderer* render) override;
    void on_update(double delta) override;
    void on_collide(Enemy* enemy) override;
private:
    Animation animation_explode;
};

